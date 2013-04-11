# Base this image on rpi-basic-image
include recipes-core/images/rpi-basic-image.bb

IMAGE_INSTALL_append = " b2g-linuxgl-rpi fbset ldd ntp ntp-utils ntpdate tzdata localedef"
DEFAULT_TIMEZONE = "Europe/Paris"

IMAGE_LINGUAS = "de-de"

# additional 1 GB space
#IMAGE_ROOTFS_EXTRA_SPACE = "1024000"
IMAGE_ROOTFS_EXTRA_SPACE = "100000"

# locales
ENABLE_BINARY_LOCALE_GENERATION = "1"
GLIBC_GENERATE_LOCALES = "en_US.UTF-8 de_DE.UTF-8"
