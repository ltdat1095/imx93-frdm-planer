SUMMARY = "Example of an out-of-tree kernel module"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://rpmsg_client.c;beginline=1;endline=10;md5=3f1047bed9e82b9b411634004bced036"

inherit module

SRC_URI = "file://rpmsg_client.c \
           file://Makefile"

S = "${WORKDIR}"
