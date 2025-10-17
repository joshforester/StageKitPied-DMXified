#!/bin/bash

# Wrapper script for being defensive with permissions on StageKit Pods so other
# processes, don't grab exclusive locks on them when skp is going through
# disconnect/reconnect cycles.

# Find the pods and set their permissions to root-only accessible.
for device in $(/opt/StageKitPied/locate_usb.sh | grep "PDP_Home_Stage_Kit" | grep "input/js" | cut -f1 -d' '); do
	/opt/StageKitPied/udev/xboxStageKitPodGuard.sh $device
done

sudo systemctl start stagekitpied
