#!/bin/bash

#
# Script for stopping QLC+ and skp in that order.  Note this assumes
# QLC+ has been started with the --web option.
#

# Set this to empty string if you do not automatically power off your DMX devices
USE_DMX_POWER_OFF="true"

if [ -n "$USE_DMX_POWER_OFF" ]; then
    # Run the command if USE_DMX_POWER_OFF is not empty
    /opt/StageKitPied/power_off_dmx_devices.sh
fi


# Now stop QLC+
QLC_PID=`ps -ef | grep qlcplus | grep web | tr -s ' ' | cut -f2 -d' '`

kill ${QLC_PID} # Kill QLC+
wait ${QLC_PID} # Wait for the program to terminate

# Now stop skp
sudo systemctl stop stagekitpied

exit 0
