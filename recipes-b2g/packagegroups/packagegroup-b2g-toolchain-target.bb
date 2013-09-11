DESCRIPTION = "Build-time dependencies for building Firefox OS for Raspberry Pi"
LICENSE = "MIT"

inherit packagegroup

PACKAGEGROUP_DISABLE_COMPLEMENTARY = "1"

RDEPENDS_${PN} += " \
    alsa-dev alsa-lib alsa-lib-dev alsa-lib-dbg \
    curl curl-dev curl-dbg \
    startup-notification startup-notification-dev startup-notification-dbg \
    libevent libevent-dev libevent-dbg \
    cairo cairo-dev cairo-dbg \
    libvpx libvpx-dev libvpx-dbg \
    libxt libxt-dev libxt-dbg\
    libxi libxi-dev libxi-dbg \
    gtk+ gtk+-dev gtk+-dbg \
    bzip2 bzip2-dev bzip2-dbg \
    dbus dbus-dev dbus-dbg \
    dbus-glib dbus-glib-dev dbus-glib-dbg \
    udev udev-dev udev-dbg libudev libudev-dev libudev-dbg"

# those are virtual packages in the real build, but that cannot be used on
# RDEPENDS
RDEPENDS_${PN} += " \
    userland userland-dev userland-dbg"
