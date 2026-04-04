FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI:append:imx93-frdm = " file://boot.cmd"
COMPATIBLE_MACHINE:append = "|imx93-frdm"
