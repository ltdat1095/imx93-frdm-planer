# Ensure i.MX93 imx-boot includes Cortex-M33 firmware container.

do_compile[depends] += " imx93-gunbot-firmware:do_deploy"

# imx-mkimage iMX93/soc.mak expects MCU image as m33_image.bin.
do_compile:prepend:mx93-nxp-bsp() {
    if [ ! -f ${DEPLOY_DIR_IMAGE}/m33_image.bin ]; then
        bbfatal "Missing ${DEPLOY_DIR_IMAGE}/m33_image.bin. Build imx93-gunbot-firmware first."
    fi

    install -m 0644 ${DEPLOY_DIR_IMAGE}/m33_image.bin ${BOOT_STAGING}/m33_image.bin
}
