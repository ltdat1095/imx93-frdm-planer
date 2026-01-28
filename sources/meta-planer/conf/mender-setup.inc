# =========================================================================
# Mender Configuration
# =========================================================================

# Enable Mender integration
INHERIT += "mender-full"

# Name of the disk image and Artifact that will be built.
MENDER_ARTIFACT_NAME = "release-1"

# Automatically create mender artifact and sdimg
MENDER_FEATURES_ENABLE:append = " mender-systemd mender-image-sd"
MENDER_FEATURES_DISABLE:append = " mender-grub mender-image-uefi mender-growfs-data"
# eMMC /dev/mmcblk0 (25GB for safe)
MENDER_STORAGE_DEVICE = "/dev/mmcblk0"
MENDER_STORAGE_TOTAL_SIZE_MB = "25000"
MENDER_DATA_PART_SIZE_MB = "400"
MENDER_BOOT_PART_SIZE_MB = "64"

# Disable default WIC to ensure only Mender's layout (.sdimg) is used
IMAGE_FSTYPES:remove = " wic wic.bmap wic.gz wic.bz2 wic.zst"

# i.MX 93 Bootloader (imx-boot) placement for Mender .sdimg
MENDER_IMAGE_BOOTLOADER_FILE = "imx-boot"
MENDER_IMAGE_BOOTLOADER_BOOTSECTOR_OFFSET = "64"
MENDER_UBOOT_STORAGE_INTERFACE = "mmc"
MENDER_UBOOT_STORAGE_DEVICE = "0"
IMAGE_BOOT_FILES:append = " boot.scr"

# --- ZST Compression & Flashing tool compatibility ---
# Ensure we compress the sdimg into the .zst format
IMAGE_FSTYPES:append = " sdimg.zst sdimg.bmap"

# Symlink Mender's layout into .wic.zst targets so UUU and NXP flashing scripts still find it
do_mender_wic_symlink() {
    if [ -f "${IMGDEPLOYDIR}/${IMAGE_NAME}.sdimg.zst" ]; then
        ln -sf "${IMAGE_NAME}.sdimg.zst" "${IMGDEPLOYDIR}/${IMAGE_LINK_NAME}.rootfs.wic.zst"
        ln -sf "${IMAGE_NAME}.sdimg.zst" "${IMGDEPLOYDIR}/${IMAGE_LINK_NAME}.wic.zst"
    fi
    if [ -f "${IMGDEPLOYDIR}/${IMAGE_NAME}.sdimg.bmap" ]; then
        ln -sf "${IMAGE_NAME}.sdimg.bmap" "${IMGDEPLOYDIR}/${IMAGE_LINK_NAME}.rootfs.wic.bmap"
        ln -sf "${IMAGE_NAME}.sdimg.bmap" "${IMGDEPLOYDIR}/${IMAGE_LINK_NAME}.wic.bmap"
    fi
}
addtask mender_wic_symlink after do_image_complete before do_build
