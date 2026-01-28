FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "file://imx93-frdm-planer.dts"

do_configure:append() {
    cp ${WORKDIR}/imx93-frdm-planer.dts ${S}/arch/arm64/boot/dts/freescale/
}
