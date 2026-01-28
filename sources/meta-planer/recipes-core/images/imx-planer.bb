SUMMARY = "Minimal i.MX93 image with planner apps"
LICENSE = "MIT"

require recipes-core/images/core-image-minimal.bb

# --- Core system & debugging ---
IMAGE_INSTALL:append = " \
    openssh \
    gdb \
    procps \
    ca-certificates \
    iproute2 \
    ethtool \
    rsync \
    util-linux \
    kernel-modules \
"

# --- Protocol ---
IMAGE_INSTALL:append = " \
    packagegroup-ros-debs \
"

# --- DEBUG TOOL ---
IMAGE_INSTALL:append = " packagegroup-core-buildessential dtc"
IMAGE_INSTALL:append = " kernel-devsrc"
IMAGE_INSTALL:append = " packagegroup-core-tools-debug"

# --- ROOTFS SIZE ---
IMAGE_ROOTFS_SIZE = "10485760"
