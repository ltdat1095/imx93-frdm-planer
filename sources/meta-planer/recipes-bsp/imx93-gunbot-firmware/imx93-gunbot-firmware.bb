SUMMARY = "i.MX93 Cortex-M33 firmware"
LICENSE = "CLOSED"

SRC_URI = "file://imx93-m33-fw.elf"

S = "${WORKDIR}"

do_install() {
    install -d ${D}${nonarch_base_libdir}/firmware
    install -m 0644 ${WORKDIR}/imx93-m33-fw.elf \
        ${D}${nonarch_base_libdir}/firmware/imx93-m33-fw.elf
}

FILES:${PN} += "${nonarch_base_libdir}/firmware/imx93-m33-fw.elf"