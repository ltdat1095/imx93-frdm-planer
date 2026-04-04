# =========================================================================
# Mender Configuration
# =========================================================================

# Enable Mender integration
INHERIT += "mender-full"
INHERIT += "mender-wic-alias"

# Name of the disk image and Artifact that will be built.
MENDER_ARTIFACT_NAME = "release-1"

# Automatically create mender artifact and sdimg.
# Explicitly enable U-Boot integration so mender_setup and related variables
# are compiled into U-Boot default environment.
MENDER_FEATURES_ENABLE:append = " mender-systemd mender-image-sd mender-uboot"
MENDER_FEATURES_DISABLE:append = " mender-grub mender-image-uefi mender-growfs-data"
# eMMC /dev/mmcblk0 (25GB for safe)
MENDER_STORAGE_DEVICE = "/dev/mmcblk0"
MENDER_STORAGE_TOTAL_SIZE_MB = "25000"
MENDER_DATA_PART_SIZE_MB = "400"
MENDER_BOOT_PART_SIZE_MB = "64"

# i.MX 93 Bootloader (imx-boot) placement for Mender .sdimg
MENDER_IMAGE_BOOTLOADER_FILE = "imx-boot"
MENDER_IMAGE_BOOTLOADER_BOOTSECTOR_OFFSET = "64"
MENDER_UBOOT_STORAGE_INTERFACE = "mmc"
MENDER_UBOOT_STORAGE_DEVICE = "0"
IMAGE_BOOT_FILES:append = " boot.scr"

# Generate Mender partition images only. .wic/.wic.zst aliases are created
# from sdimg/sdimg.zst by mender-wic-alias.bbclass.
IMAGE_FSTYPES:append = " sdimg.zst"
