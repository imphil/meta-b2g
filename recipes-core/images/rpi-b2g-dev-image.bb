DESCRIPTION = "A development image for debugging Firefox OS on  Raspberry Pi"
include rpi-b2g-image.bb

IMAGE_INSTALL_append = " ldd strace nfs-export-root bash gdbserver vim"

IMAGE_FEATURES += " tools-debug dbg-pkgs dev-pkgs debug-tweaks nfs-server"
