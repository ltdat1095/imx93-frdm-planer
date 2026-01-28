#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x2c635209, "module_layout" },
	{ 0x3ee2b36d, "class_destroy" },
	{ 0x1ef20f9f, "unregister_rpmsg_driver" },
	{ 0x86bbf3d3, "__register_rpmsg_driver" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x9b0fb107, "__class_create" },
	{ 0x92997ed8, "_printk" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xaf88e69b, "kmem_cache_alloc_trace" },
	{ 0x30a93ed, "kmalloc_caches" },
	{ 0xa3552b68, "rpmsg_send" },
	{ 0xcf2a881c, "device_create" },
	{ 0xabac4112, "cdev_add" },
	{ 0x51b1c11d, "cdev_init" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0xc3fcadfd, "devm_kmalloc" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0x1000e51, "schedule" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x800473f, "__cond_resched" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x37a0cba, "kfree" },
	{ 0x37ce6741, "cdev_del" },
	{ 0xdff7669f, "device_destroy" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0xf0311d7, "_dev_err" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x167c5967, "print_hex_dump" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xec2dcbc3, "_dev_info" },
	{ 0x4c9f47a5, "current_task" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "rpmsg_core");

MODULE_ALIAS("rpmsg:rpmsg-client");

MODULE_INFO(srcversion, "E4B645C49923C5A069C5FCD");
