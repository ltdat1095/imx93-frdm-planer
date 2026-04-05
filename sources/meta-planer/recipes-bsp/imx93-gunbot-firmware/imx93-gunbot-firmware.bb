SUMMARY = "i.MX93 Cortex-M33 firmware"
LICENSE = "CLOSED"
DEPENDS += "binutils-native"

SRC_URI = "file://imx93-m33-fw.elf"

S = "${WORKDIR}"

inherit deploy

do_install() {
    install -d ${D}${nonarch_base_libdir}/firmware
    install -m 0644 ${WORKDIR}/imx93-m33-fw.elf \
        ${D}${nonarch_base_libdir}/firmware/imx93-m33-fw.elf
}

do_deploy() {
    install -d ${DEPLOYDIR}
    install -m 0644 ${WORKDIR}/imx93-m33-fw.elf ${DEPLOYDIR}/imx93-m33-fw.elf
    ${TARGET_PREFIX}objcopy -O binary \
        ${WORKDIR}/imx93-m33-fw.elf ${DEPLOYDIR}/m33_image.bin
}

addtask deploy after do_install before do_build

FILES:${PN} += "${nonarch_base_libdir}/firmware/imx93-m33-fw.elf"
INSANE_SKIP:${PN} += "arch"
