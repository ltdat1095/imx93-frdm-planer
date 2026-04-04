FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
FILESPATH:prepend := "/home/ltdat/Desktop/autonomous_explorer/imx93-frdm-yocto/frdm-imx93/workspace/sources/linux-imx/oe-local-files:"
# srctreebase: /home/ltdat/Desktop/autonomous_explorer/imx93-frdm-yocto/frdm-imx93/workspace/sources/linux-imx

inherit externalsrc
# NOTE: We use pn- overrides here to avoid affecting multiple variants in the case where the recipe uses BBCLASSEXTEND
EXTERNALSRC:pn-linux-imx = "/home/ltdat/Desktop/autonomous_explorer/imx93-frdm-yocto/frdm-imx93/workspace/sources/linux-imx"
SRCTREECOVEREDTASKS = "do_validate_branches do_kernel_checkout do_fetch do_unpack do_kernel_configcheck"

do_patch[noexec] = "1"

do_configure:append() {
    cp ${B}/.config ${S}/.config.baseline
    ln -sfT ${B}/.config ${S}/.config.new
}

do_kernel_configme:prepend() {
    if [ -e ${S}/.config ]; then
        mv ${S}/.config ${S}/.config.old
    fi
}

do_configure:append() {
    if [ ${@oe.types.boolean(d.getVar("KCONFIG_CONFIG_ENABLE_MENUCONFIG"))} = True ]; then
        cp ${KCONFIG_CONFIG_ROOTDIR}/.config ${S}/.config.baseline
        ln -sfT ${KCONFIG_CONFIG_ROOTDIR}/.config ${S}/.config.new
    fi
}

# initial_rev .: 15ccf1048117da9c5c050382a9b6e1e222a1e1b3
