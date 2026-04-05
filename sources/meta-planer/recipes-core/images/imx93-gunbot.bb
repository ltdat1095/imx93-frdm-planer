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

# --- Applications ---
IMAGE_INSTALL:append = " \
    imx93-gunbot-firmware \
    kernel-module-nxp-wlan \
    nxp-wlan-sdk \
    firmware-nxp-wifi-nxpiw612-sdio \
    wifi-autoconnect \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-ugly \
    gstreamer1.0-plugins-base \
    gstreamer1.0-libav \
    gstreamer1.0 \
"

# --- DEBUG TOOL ---
IMAGE_INSTALL:append = " packagegroup-core-buildessential dtc"
IMAGE_INSTALL:append = " kernel-devsrc"
IMAGE_INSTALL:append = " packagegroup-core-tools-debug"

# --- ROOTFS SIZE ---
IMAGE_ROOTFS_SIZE = "10485760"
