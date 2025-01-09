#!/bin/bash

# Check if the script is running as root, since service install requires it.
if [ "$EUID" -ne 0 ]; then
    echo "This script must be run as root." 1>&2
    exit 1
fi

SKP_INSTALL_DIR=/opt/StageKitPied

# Setup the program in its place.
mkdir -p ${SKP_INSTALL_DIR}
cp skp ${SKP_INSTALL_DIR}
cp lights.ini ${SKP_INSTALL_DIR}
cp leds1.ini ${SKP_INSTALL_DIR}
cp leds2.ini ${SKP_INSTALL_DIR}
cp leds3.ini ${SKP_INSTALL_DIR}
cp leds4.ini ${SKP_INSTALL_DIR}
cp leds5.ini ${SKP_INSTALL_DIR}

chmod u+x ${SKP_INSTALL_DIR}/skp

# Setup the service.
cp stagekitpied.service /lib/systemd/system/
systemctl daemon-reload
systemctl enable stagekitpied.service

echo "Startup service installed.  You will need to reboot to see it in action."
