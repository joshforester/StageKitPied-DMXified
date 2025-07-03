#!/bin/bash

#
# Script for launching skp and QLC+ in the proper order.  Note this script
# relies on .config/autostart/qlcplus.desktop being set and restarts lightdm
# in order to trigger the QLC+ start.
#

SKPD_PATH=/opt/StageKitPied

# Set this to empty string if you do not automatically power on your DMX devices
USE_DMX_POWER_ON="true"

# Set this to empty string if you do not have the led_drumkit software installed
USE_LED_DRUMKIT="true"

# Set this to empty string if you do not use file buttons.
USE_FILE_BUTTONS="true"

if [ -n "$USE_DMX_POWER_ON" ]; then
    # Run the command if USE_DMX_POWER_ON is not empty
    $SKPD_PATH/power_on_dmx_devices.sh
fi

if [ -n "$USE_FILE_BUTTONS" ]; then
	# Stop all button services to clear their state.
	sudo systemctl stop active_touch_file_buttons
	sudo systemctl stop switch_touch_file_buttons

	# Turn all buttons "off" by removing temp files.
	button_file_numbers=(12 19 20 25)
	for button_file_number in "${button_file_numbers[@]}"; do
    	if [ -f "$SKPD_PATH/button$button_file_number.tmp" ]; then
        	rm $SKPD_PATH/button$button_file_number.tmp 
    	fi
	done

	# Start all button services after clearing their state.
	sudo systemctl start active_touch_file_buttons
	sudo systemctl start switch_touch_file_buttons
fi

if [ -n "$USE_LED_DRUMKIT" ]; then
    # Run the command if USE_LED_DRUMKIT is not empty
    sudo systemctl start leddrumkit 
fi

sudo $SKPD_PATH/skp_wrapper.sh

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
