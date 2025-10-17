#!/bin/bash

#
# Script for stopping QLC+ and skp in that order. 
#

# Set this to empty string if you do not automatically power off your DMX devices
USE_DMX_POWER_OFF="true"

# Set this to empty string if you do not have the led_drumkit software installed
USE_LED_DRUMKIT="true"

if [ -n "$USE_LED_DRUMKIT" ]; then
    # Run the command if USE_LED_DRUMKIT is not empty
    sudo systemctl stop leddrumkit 
fi

if [ -n "$USE_DMX_POWER_OFF" ]; then
    # Run the command if USE_DMX_POWER_OFF is not empty
    /opt/StageKitPied/power_off_dmx_devices.sh
fi


# Stop QLC+ Container
sudo systemctl stop qlcplus-podman

# Now stop skp
sudo systemctl stop stagekitpied

exit 0
