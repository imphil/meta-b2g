DESCRIPTION = "Firefox OS (Boot2Gecko) LinuxGL for RaspberryPI"

COMPATIBLE_MACHINE = "raspberrypi"

# unfortunately RDEPENDS does not support virtual packages, so we need
# to specify the full package name.
RDEPENDS_${PN} += "userland"

SRCREV = "801ba75ac563"

#PV = "dev"
#PR = "r1"
#PR_append = "+hg${SRCREV}"

MOZCONFIG_FILE = "mozconfig-b2g-linuxgl-rpi"

require b2g-linuxgl.inc
