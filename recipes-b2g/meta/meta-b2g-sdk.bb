DESCRIPTION = "SDK for the development of Firefox OS for Raspberry Pi. It \
               contains the toolchain and all development libraries."
SUMMARY = "SDK for the development of Firefox OS for Raspberry Pi"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/LICENSE;md5=3f40d7994397109285ec7b81fdeb3b58 \
                    file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

TOOLCHAIN_TARGET_TASK ?= "packagegroup-b2g-toolchain-target"
TOOLCHAIN_HOST_TASK = " \
    nativesdk-packagegroup-sdk-host \
    packagegroup-cross-canadian-${TRANSLATED_TARGET_ARCH} \
    "

require recipes-core/meta/meta-toolchain.bb

# We replace the function in meta-toolchain.bb withour own one, specific to the
# purpose of building Mozilla/Firefox OS with this SDK
toolchain_create_sdk_env_script () {
    # Create environment setup script
    script=${1:-${SDK_OUTPUT}/${SDKPATH}/environment-setup-${REAL_MULTIMACH_TARGET_SYS}}
    rm -f $script
    touch $script


    echo 'export PATH=${SDKPATHNATIVE}${bindir_nativesdk}/${REAL_MULTIMACH_TARGET_SYS}:$PATH' >> $script

    echo 'export CROSS_COMPILE="1"' >> $script
    echo 'export CONFIGURE_ARGS="--target=${TARGET_SYS} --host=${TARGET_SYS} --build=${SDK_ARCH}-linux --with-libtool-sysroot=${SDKTARGETSYSROOT}"' >> $script

    echo '' >> $script
    echo '# target' >> $script
    echo 'export PKG_CONFIG_SYSROOT_DIR=${SDKTARGETSYSROOT}' >> $script
    echo 'export PKG_CONFIG_PATH=${SDKTARGETSYSROOT}${libdir}/pkgconfig' >> $script
    echo 'export CONFIG_SITE=${SDKPATH}/site-config-${REAL_MULTIMACH_TARGET_SYS}' >> $script
    echo 'export CC="${TARGET_PREFIX}gcc ${TARGET_CC_ARCH} --sysroot=${SDKTARGETSYSROOT}"' >> $script
    echo 'export CXX="${TARGET_PREFIX}g++ ${TARGET_CC_ARCH} --sysroot=${SDKTARGETSYSROOT}"' >> $script
    echo 'export CPP="${TARGET_PREFIX}gcc -E ${TARGET_CC_ARCH} --sysroot=${SDKTARGETSYSROOT}"' >> $script
    echo 'export AS="${TARGET_PREFIX}as ${TARGET_AS_ARCH}"' >> $script
    echo 'export LD="${TARGET_PREFIX}ld ${TARGET_LD_ARCH} --sysroot=${SDKTARGETSYSROOT}"' >> $script
    echo 'export GDB=${TARGET_PREFIX}gdb' >> $script
    echo 'export STRIP=${TARGET_PREFIX}strip' >> $script
    echo 'export RANLIB=${TARGET_PREFIX}ranlib' >> $script
    echo 'export OBJCOPY=${TARGET_PREFIX}objcopy' >> $script
    echo 'export OBJDUMP=${TARGET_PREFIX}objdump' >> $script
    echo 'export AR=${TARGET_PREFIX}ar' >> $script
    echo 'export NM=${TARGET_PREFIX}nm' >> $script

    echo 'export CPPFLAGS="-I${SDKTARGETSYSROOT}/usr/include ${TARGET_CPPFLAGS}"' >> $script

    echo '' >> $script
    echo '# build host' >> $script
    echo 'export HOST_CC="gcc"' >> $script
    echo 'export HOST_CXX="g++"' >> $script
    echo 'export HOST_RANLIB="ranlib"' >> $script
    echo 'export HOST_AR="ar"' >> $script
}
