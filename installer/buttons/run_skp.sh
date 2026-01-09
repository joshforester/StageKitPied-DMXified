#!/bin/bash

#
# Script for launching skp and QLC+ in the proper order. 
#

SKPD_DIR=/opt/StageKitPied
SKPD_INPUT_DIR=${SKPD_PATH}/input
SKPD_OPTIONAL_DIR=${SKPD_PATH}/optional
SKPD_UDEV_DIR=${SKPD_PATH}/udev

# Set this to empty string if you do not automatically power on your DMX devices
USE_DMX_POWER_ON="true"

# Set this to empty string if you do not have the led_drumkit software installed
USE_LED_DRUMKIT="true"

# Set this to empty string if you do not use file buttons.
USE_FILE_BUTTONS="true"

# Reset USB to DMX adapter.
$SKPD_UDEV_DIR/resetFtdiDmx.sh

if [ -n "$USE_DMX_POWER_ON" ]; then
    # Run the command if USE_DMX_POWER_ON is not empty
    $SKPD_OPTIONAL_DIR/power_on_dmx_devices.sh
fi

if [ -n "$USE_FILE_BUTTONS" ]; then
	# Stop all button services to clear their state.
	systemctl stop active_touch_file_buttons
	systemctl stop switch_touch_file_buttons

	# Turn all buttons "off" by removing temp files.
	button_file_numbers=(12 19 20 25)
	for button_file_number in "${button_file_numbers[@]}"; do
    		if [ -f "$SKPD_INPUT_DIR/button$button_file_number.tmp" ]; then
        		rm $SKPD_INPUT_DIR/button$button_file_number.tmp 
    		fi
	done

	# Start all button services after clearing their state.
	systemctl start active_touch_file_buttons
	systemctl start switch_touch_file_buttons
fi

if [ -n "$USE_LED_DRUMKIT" ]; then
    # Run the command if USE_LED_DRUMKIT is not empty
    systemctl start leddrumkit 
fi

systemctl start qlcplus-podman

$SKPD_OPTIONAL_DIR/skp_wrapper.sh

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

exit 0
