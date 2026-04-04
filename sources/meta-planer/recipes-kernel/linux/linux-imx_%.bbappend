FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " \
    file://imx93-frdm-planer.dts \
    file://imx93-gunbot.dts \
"

do_configure:append() {
    cp ${WORKDIR}/imx93-frdm-planer.dts ${S}/arch/arm64/boot/dts/freescale/
    cp ${WORKDIR}/imx93-gunbot.dts ${S}/arch/arm64/boot/dts/freescale/
}
