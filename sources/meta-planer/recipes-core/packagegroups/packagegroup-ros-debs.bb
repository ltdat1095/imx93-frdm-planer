SUMMARY = "ROS dependencies"
PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGES = "${PN}"

RDEPENDS:${PN} = "\
    ros-core \
    common-interfaces \
    python3 \
"