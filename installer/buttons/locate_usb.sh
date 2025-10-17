#!/bin/bash

#
# This script will display device files (i.e. /dev/ttyUSB0) and their associated
# descriptors (i.e. "Performance_Designed_Products_PDP_Home_Stage_Kit_003BB097")
# while they are currently not "in use" by an application.  That is, when 
# StageKitPied-DMXified is running, these Stage Kit devices will NOT show up in
# the list.  When it is not running, you may use this to confirm they are
# connected and communicating with the Raspberry Pi.
#
# Note:  The serial adapter currently shows even while running.
#
for sysdevpath in $(find /sys/bus/usb/devices/usb*/ -name dev); do
    (
        syspath="${sysdevpath%/dev}"
        devname="$(udevadm info -q name -p $syspath)"
        [[ "$devname" == "bus/"* ]] && exit
        eval "$(udevadm info -q property --export -p $syspath)"
        [[ -z "$ID_SERIAL" ]] && exit
        echo "/dev/$devname - $ID_SERIAL"
    )
done

echo ""
echo "Showing Serial Adapter and any other potential conflictin adapter links (DMX Adapter can sometimes be shown here if using the same FTDI chip)"
ls -l /dev/ftdi*
