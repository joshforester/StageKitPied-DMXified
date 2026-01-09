#!/bin/bash

# Wrapper script for being defensive with permissions on StageKit Pods so other
# processes, don't grab exclusive locks on them when skp is going through
# disconnect/reconnect cycles.

SKPD_DIR=/opt/StageKitPied
SKPD_OPTIONAL_DIR=${SKPD_PATH}/optional
SKPD_UDEV_DIR=${SKPD_PATH}/udev

# Find the pods and set their permissions to root-only accessible.
for device in $(SKPD_OPTIONAL_DIR/locate_usb.sh | grep "PDP_Home_Stage_Kit" | grep "input/js" | cut -f1 -d' '); do
	$SKPD_UDEV_DIR/xboxStageKitPodGuard.sh $device
done

sudo systemctl start stagekitpied

exit 0
