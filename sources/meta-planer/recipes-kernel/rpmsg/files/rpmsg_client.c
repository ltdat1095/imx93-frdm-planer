// SPDX-License-Identifier: GPL-2.0-only
/*
 * Remote processor messaging - sample client driver
 *
 * Copyright (C) 2011 Texas Instruments, Inc.
 * Copyright (C) 2011 Google, Inc.
 *
 * Ohad Ben-Cohen <ohad@wizery.com>
 * Brian Swetland <swetland@google.com>
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/rpmsg.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/list.h>

#define END_POINT_NAME "health_monitor"
#define MSG		"Hello world!"
#define MAX_RPMSG_BUF_SIZE	512
#define RPMSG_MAX_CLIENTS	16

struct rpmsg_packet {
	int id;
	int type;
	int status;
};

struct rpmsg_client_msg {
	struct list_head node;
	size_t len;
	u8 data[];
};

struct instance_data {
	struct rpmsg_device *rpdev;
	struct cdev cdev;
	struct device *dev;
	struct mutex lock;
	wait_queue_head_t readq;
	struct list_head msg_list;
};

static dev_t rpmsg_dev_t;
static struct class *rpmsg_class;
static int rpmsg_client_count = 0;

static int rpmsg_client_open(struct inode *inode, struct file *filp)
{
	struct instance_data *idata = container_of(inode->i_cdev, struct instance_data, cdev);
	filp->private_data = idata;
	dev_info(idata->dev, "rpmsg_client: opened by pid %d\n", current->pid);
	return 0;
}

static int rpmsg_client_release(struct inode *inode, struct file *filp)
{
	struct instance_data *idata = filp->private_data;
	dev_info(idata->dev, "rpmsg_client: released by pid %d\n", current->pid);
	return 0;
}

static ssize_t rpmsg_client_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	struct instance_data *idata = filp->private_data;
	struct rpmsg_client_msg *msg;
	int ret = 0;

	if (list_empty(&idata->msg_list)) {
		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;
		ret = wait_event_interruptible(idata->readq, !list_empty(&idata->msg_list));
		if (ret)
			return ret;
	}

	mutex_lock(&idata->lock);
	if (list_empty(&idata->msg_list)) {
		mutex_unlock(&idata->lock);
		return -EAGAIN;
	}

	msg = list_first_entry(&idata->msg_list, struct rpmsg_client_msg, node);

	if (count > msg->len)
		count = msg->len;

	if (copy_to_user(buf, msg->data, count)) {
		dev_err(idata->dev, "rpmsg_client: copy_to_user failed\n");
		ret = -EFAULT;
		goto out;
	}

	dev_info(idata->dev, "rpmsg_client: read %zu bytes\n", count);
	print_hex_dump(KERN_INFO, "rpmsg_client_rx: ", DUMP_PREFIX_NONE, 16, 1, msg->data, count, true);

	ret = count;

	list_del(&msg->node);
	kfree(msg);

out:
	mutex_unlock(&idata->lock);
	return ret;
}

static ssize_t rpmsg_client_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	struct instance_data *idata = filp->private_data;
	void *kbuf;
	int ret;
	struct rpmsg_packet *pkt;

	if (count != sizeof(struct rpmsg_packet)) {
		dev_err(idata->dev, "rpmsg_client: invalid packet size: %zu, expected %zu\n", count, sizeof(struct rpmsg_packet));
		return -EMSGSIZE;
	}

	kbuf = kzalloc(count, GFP_KERNEL);
	if (!kbuf)
		return -ENOMEM;

	if (copy_from_user(kbuf, buf, count)) {
		dev_err(idata->dev, "rpmsg_client: copy_from_user failed\n");
		kfree(kbuf);
		return -EFAULT;
	}

	pkt = (struct rpmsg_packet *)kbuf;
	dev_info(idata->dev, "rpmsg_client: writing packet: id=%d, type=%d, status=%d\n", pkt->id, pkt->type, pkt->status);
	print_hex_dump(KERN_INFO, "rpmsg_client_tx: ", DUMP_PREFIX_NONE, 16, 1, kbuf, count, true);

	ret = rpmsg_send(idata->rpdev->ept, kbuf, count);
	if (ret) {
		dev_err(&idata->rpdev->dev, "rpmsg_send failed: %d\n", ret);
	} else {
		ret = count;
	}

	kfree(kbuf);
	return ret;
}

static const struct file_operations rpmsg_client_fops = {
	.owner		= THIS_MODULE,
	.read		= rpmsg_client_read,
	.write		= rpmsg_client_write,
	.open		= rpmsg_client_open,
	.release	= rpmsg_client_release,
};

static int rpmsg_sample_cb(struct rpmsg_device *rpdev, void *data, int len,
						void *priv, u32 src)
{
	struct instance_data *idata = dev_get_drvdata(&rpdev->dev);
	struct rpmsg_client_msg *msg;
	struct rpmsg_packet *pkt;

	dev_info(&rpdev->dev, "incoming msg from %d: len %d\n", src, len);
	if (len == sizeof(struct rpmsg_packet)) {
		pkt = (struct rpmsg_packet *)data;
		dev_info(&rpdev->dev, "rpmsg_packet received: id=%d, type=%d, status=%d\n", pkt->id, pkt->type, pkt->status);
	}
	print_hex_dump(KERN_INFO, "rpmsg_cb: ", DUMP_PREFIX_NONE, 16, 1, data, len, true);

	msg = kmalloc(sizeof(*msg) + len, GFP_ATOMIC);
	if (!msg) {
		dev_err(&rpdev->dev, "failed to allocate message\n");
		return -ENOMEM;
	}

	msg->len = len;
	memcpy(msg->data, data, len);

	mutex_lock(&idata->lock);
	list_add_tail(&msg->node, &idata->msg_list);
	mutex_unlock(&idata->lock);

	wake_up_interruptible(&idata->readq);

	return 0;
}

static int rpmsg_sample_probe(struct rpmsg_device *rpdev)
{
	int ret;
	struct instance_data *idata;
	dev_t devt;

	dev_info(&rpdev->dev, "new channel: 0x%x -> 0x%x!\n",
					rpdev->src, rpdev->dst);

	idata = devm_kzalloc(&rpdev->dev, sizeof(*idata), GFP_KERNEL);
	if (!idata)
		return -ENOMEM;

	idata->rpdev = rpdev;
	mutex_init(&idata->lock);
	INIT_LIST_HEAD(&idata->msg_list);
	init_waitqueue_head(&idata->readq);

	dev_set_drvdata(&rpdev->dev, idata);

	if (rpmsg_client_count >= RPMSG_MAX_CLIENTS) {
		dev_err(&rpdev->dev, "too many clients\n");
		return -ENOSPC;
	}

	devt = MKDEV(MAJOR(rpmsg_dev_t), rpmsg_client_count);
	cdev_init(&idata->cdev, &rpmsg_client_fops);
	idata->cdev.owner = THIS_MODULE;

	ret = cdev_add(&idata->cdev, devt, 1);
	if (ret) {
		dev_err(&rpdev->dev, "cdev_add failed: %d\n", ret);
		return ret;
	}

	idata->dev = device_create(rpmsg_class, &rpdev->dev, devt, NULL, "rpmsg_client%d", rpmsg_client_count);
	if (IS_ERR(idata->dev)) {
		ret = PTR_ERR(idata->dev);
		dev_err(&rpdev->dev, "device_create failed: %d\n", ret);
		cdev_del(&idata->cdev);
		return ret;
	}
	
	dev_info(&rpdev->dev, "created device /dev/rpmsg_client%d\n", rpmsg_client_count);

	rpmsg_client_count++;

	/* send a message to our remote processor */
	ret = rpmsg_send(rpdev->ept, MSG, strlen(MSG));
	if (ret) {
		dev_err(&rpdev->dev, "rpmsg_send failed: %d\n", ret);
	}

	return 0;
}

static void rpmsg_sample_remove(struct rpmsg_device *rpdev)
{
	struct instance_data *idata = dev_get_drvdata(&rpdev->dev);
	struct rpmsg_client_msg *msg, *tmp;

	device_destroy(rpmsg_class, idata->cdev.dev);
	cdev_del(&idata->cdev);

	mutex_lock(&idata->lock);
	list_for_each_entry_safe(msg, tmp, &idata->msg_list, node) {
		list_del(&msg->node);
		kfree(msg);
	}
	mutex_unlock(&idata->lock);

	dev_info(&rpdev->dev, "rpmsg client driver is removed\n");
}

static struct rpmsg_device_id rpmsg_driver_sample_id_table[] = {
	{ .name	= "health_monitor" },
	{ },
};
MODULE_DEVICE_TABLE(rpmsg, rpmsg_driver_sample_id_table);
static struct rpmsg_driver rpmsg_msg_client = {
	.drv.name	= KBUILD_MODNAME,
	.id_table	= rpmsg_driver_sample_id_table,
	.probe		= rpmsg_sample_probe,
	.callback	= rpmsg_sample_cb,
	.remove		= rpmsg_sample_remove,
};

static int __init rpmsg_client_init(void)
{
	int ret;

	ret = alloc_chrdev_region(&rpmsg_dev_t, 0, RPMSG_MAX_CLIENTS, END_POINT_NAME);
	if (ret < 0) {
		pr_err("rpmsg_client: failed to allocate char dev region\n");
		return ret;
	}

	rpmsg_class = class_create(END_POINT_NAME);
	if (IS_ERR(rpmsg_class)) {
		pr_err("rpmsg_client: failed to create class\n");
		unregister_chrdev_region(rpmsg_dev_t, RPMSG_MAX_CLIENTS);
		return PTR_ERR(rpmsg_class);
	}

	return register_rpmsg_driver(&rpmsg_msg_client);
}

static void __exit rpmsg_client_exit(void)
{
	unregister_rpmsg_driver(&rpmsg_msg_client);
	class_destroy(rpmsg_class);
	unregister_chrdev_region(rpmsg_dev_t, RPMSG_MAX_CLIENTS);
}

module_init(rpmsg_client_init);
module_exit(rpmsg_client_exit);

MODULE_DESCRIPTION("Remote processor messaging sample client driver");
MODULE_LICENSE("GPL v2");
