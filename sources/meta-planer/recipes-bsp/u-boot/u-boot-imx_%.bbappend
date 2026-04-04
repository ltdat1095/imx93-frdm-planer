FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

BOOTENV_SIZE:imx93-frdm = "0x20000"
MENDER_UBOOT_AUTO_CONFIGURE:imx93-frdm = "1"

MENDER_UBOOT_ENV_STORAGE_DEVICE_OFFSET:imx93-frdm = "0x700000"
MENDER_UBOOT_ENV_STORAGE_DEVICE_OFFSET_2:imx93-frdm = "0x740000"
SRC_URI:append:imx93-frdm = " file://0001-configs-imx93_11x11_frdm-enable-Mender-support.patch"
SRC_URI:append:imx93-frdm = " file://0002-configs-imx93_11x11_frdm-enable-redundant-env.patch"
SRC_URI:append:imx93-frdm = " file://0003-configs-imx93_11x11_frdm-enable-bootcount.patch"
SRC_URI:append:imx93-frdm = " file://uboot_auto_patch.sh"
SRC_URI:append:imx93-frdm = " file://0004-mender-allow-env-is-nowhere-with-mmc-ubi.patch"
