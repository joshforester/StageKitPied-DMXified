#!/bin/bash

#
# Script for launching skp and QLC+ in the proper order.  Note this script
# relies on .config/autostart/qlcplus.desktop being set and restarts lightdm
# in order to trigger the QLC+ start.
#


sudo /opt/StageKitPied/skp_wrapper.sh

# Loop until the service is active
while true; do
    # Check if the service status is 'active'
    status=$(systemctl status stagekitpied | grep Active | cut -f2 -d':' | cut -f2 -d' ')
    # If status is not 'active', sleep for 1 second
    if [[ "$status" != "active" ]]; then
        sleep 1
    else
        # Exit the loop once the service is active
        break
    fi
done

sudo systemctl restart lightdm
