SUMMARY = "Auto-connect Wi-Fi service for boot"
LICENSE = "CLOSED"

SRC_URI = " \
    file://wifi-autoconnect.service \
    file://wifi-autoconnect.sh \
    file://wpa_supplicant.conf \
"

S = "${WORKDIR}"

inherit systemd

RDEPENDS:${PN} += " \
    wpa-supplicant \
    busybox \
    iproute2 \
"

do_install() {
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/wifi-autoconnect.service ${D}${systemd_system_unitdir}/wifi-autoconnect.service

    install -d ${D}${sbindir}
    install -m 0755 ${WORKDIR}/wifi-autoconnect.sh ${D}${sbindir}/wifi-autoconnect.sh

    install -d ${D}${sysconfdir}/wifi
    install -m 0600 ${WORKDIR}/wpa_supplicant.conf ${D}${sysconfdir}/wifi/wpa_supplicant.conf
}

SYSTEMD_SERVICE:${PN} = "wifi-autoconnect.service"
SYSTEMD_AUTO_ENABLE = "enable"

FILES:${PN} += " \
    ${systemd_system_unitdir}/wifi-autoconnect.service \
    ${sbindir}/wifi-autoconnect.sh \
    ${sysconfdir}/wifi/wpa_supplicant.conf \
"
