DESCRIPTION ?= "Firefox OS (Boot2Gecko) LinuxGL"

LICENSE = "MPLv2 | GPLv2+ | LGPLv2.1+"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MPL-2.0;md5=815ca599c9df247a0c7f619bab123dad"

DEPENDS += "alsa-lib curl startup-notification libevent cairo libvpx \
            virtual/egl gnu-config-native virtual/libintl libxt libxi \
            zip-native libidl-native gtk+ bzip2 \
            gcc-runtime dbus dbus-glib python-native"
RDEPENDS_${PN} +=  "liberation-fonts ttf-dejavu-sans ttf-dejavu-sans-mono"

FILESEXTRAPATHS_prepend := "${FILE_DIRNAME}/b2g-linuxgl:"

SRC_URI = "hg://hg.mozilla.org/;module=mozilla-central;rev=${SRCREV} \
           file://${MOZCONFIG_FILE} \
           file://0001-1_configure.diff \
           file://0002-2_chromium.diff \
           file://0003-3_plugins_ipdl_include.diff \
           file://0004-4_gfx_platform.diff \
           file://0005-6_widget_port.diff \
           file://0006-ua_string_append.diff \
           file://0007-9_fontconfig_link.diff \
           file://0008-16_fonts_gfx_android_match.diff \
           file://0009-1_xconfig_base.diff \
           file://0010-2_xcompile_basic.diff \
           file://0011-2_xwindow_simple.diff \
           file://0012-udev_configure.diff \
           file://0013-14_input_dispatcher.diff \
           file://0014-b2g_default_prefs.diff \
           file://0015-b2g_console_fps.diff \
           file://0016-glCursor_pointer.diff \
           file://0017-evdev_kbd_handler.diff \
           file://0018-rasppi_b2g_fix.diff \
           file://0019-adjust_screen_size_1280x1024.diff \
           file://0020-tree_freetype_without_bzip.patch \
           file://0021-egl_preload_glesv2.patch \
           file://0022-workaround_wrong_vcpath.diff \
           file://0023-remove_android_log.diff \
           file://0024-6_widget_port_compilefixes.diff \
           file://0025-force_enable_drawing_without_bootscreen.diff"

S = "${WORKDIR}/mozilla-central"

# some of the patches contain git special semantics (such as file copies), which
# need to be applied using a patch tool that understands that.
PATCHTOOL = "git"

# Disable ARM Thumb instructions
# http://lists.linuxtogo.org/pipermail/openembedded-core/2011-July/007069.html
#
# Mozilla supports Thumb-2 instructions, but not Thumb-1 (the original ones)
# (Mozilla bug 792134). If you have a CPU which supports Thumb-2, you can
# comment the following line.
ARM_INSTRUCTION_SET = "arm"

inherit gettext pkgconfig

EXTRA_OECONF = "--target=${TARGET_SYS} --host=${BUILD_SYS} \
                --build=${BUILD_SYS} --prefix=${prefix}"

EXTRA_OEMAKE = "installdir=${libdir}/${PN}"

export CROSS_COMPILE = "1"
export MOZCONFIG = "${WORKDIR}/${MOZCONFIG_FILE}"
export MOZ_OBJDIR = "${S}/objdir"

export CONFIGURE_ARGS = "${EXTRA_OECONF}"
export HOST_CC = "${BUILD_CC}"
export HOST_CXX = "${BUILD_CXX}"
export HOST_CFLAGS = "${BUILD_CFLAGS}"
export HOST_CXXFLAGS = "${BUILD_CXXFLAGS}"
export HOST_LDFLAGS = "${BUILD_LDFLAGS}"
export HOST_RANLIB = "${BUILD_RANLIB}"
export HOST_AR = "${BUILD_AR}"
# Set the host libIDL stuff correctly.
export HOST_LIBIDL_CONFIG="PKG_CONFIG_PATH=${STAGING_LIBDIR_NATIVE}/pkgconfig pkg-config libIDL-2.0"
# Due to sysroot we need to sed out references to the target staging
# when building the native version of xpidl Symptons of the failure
# include "gthread.h:344: error: size of array 'type name' is negative"
export HOST_LIBIDL_CFLAGS="`${HOST_LIBIDL_CONFIG} --cflags | sed -e s:${STAGING_DIR_TARGET}::g`"
export HOST_LIBIDL_LIBS="`${HOST_LIBIDL_CONFIG} --libs`"

do_configure() {
    # Put PARALLEL_MAKE into mozconfig
    if [ ! -z "${PARALLEL_MAKE}" ] ; then
        echo mk_add_options MOZ_MAKE_FLAGS=\"${PARALLEL_MAKE}\" \
            >> ${MOZCONFIG}
    fi

    if [ -e ${MOZ_OBJDIR}/Makefile ] ; then
        oe_runmake -f client.mk ${MOZ_OBJDIR}/Makefile \
                    ${MOZ_OBJDIR}/config.status
    fi

    sed -i -e 's,@prefix@,${prefix},g' \
           -e 's,@STAGING_INCDIR@,${STAGING_INCDIR},g' \
           -e 's,@STAGING_DIR_TARGET@,${STAGING_DIR_TARGET},g' \
           ${MOZCONFIG}
}

do_compile() {
    oe_runmake -f client.mk build_all
}

do_install() {
    # We use Mozilla's "make package", which creates a gz file and then unpack
    # it again. This is the recommended way.
    # "On POSIX platforms, you can install your build to the system by running
    #  gmake install. However, this is not recommended, and you should instead
    #  follow the steps below to make a tarball, then unpack the tarball."
    # -- https://developer.mozilla.org/en-US/docs/Build_and_Install

    cd ${MOZ_OBJDIR}
    oe_runmake package
    install -d ${D}${libdir}
    install -d ${D}${bindir}
    tar xzf ${MOZ_OBJDIR}/dist/b2g-*.tar.gz -C ${D}${libdir}
    ln -s ../lib/b2g/b2g ${D}${bindir}
}

FILES_${PN} = "${libdir}/b2g \
               ${bindir}/b2g"

FILES_${PN}-dbg += "${libdir}/b2g/.debug \
                    ${libdir}/b2g/*/.debug \
                    ${libdir}/b2g/*/*/.debug \
                    ${libdir}/b2g/*/*/*/.debug \
                    ${libdir}/b2g-devel-*/*/.debug \
                    ${libdir}/b2g-devel-*/*/*/.debug \
                    ${libdir}/b2g-devel-*/*/*/*/.debug \
                    ${bindir}/.debug"

# We don't build XUL as system shared lib, so we can mark all libs as private.
# This prevents the libraries from being used to resolve runtime dependencies
# of other packages.
PRIVATE_LIBS = "libfreebl3.so \
                libmozsqlite3.so \
                libnss3.so \
                libnssdbm3.so \
                libplc4.so \
                libsmime3.so \
                libssl3.so \
                libxul.so \
                libmozalloc.so \
                libnspr4.so \
                libnssckbi.so \
                libnssutil3.so \
                libplds4.so \
                libsoftokn3.so \
                libxpcom.so"
