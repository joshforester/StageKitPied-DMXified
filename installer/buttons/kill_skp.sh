#!/bin/bash

#
# Script for stopping QLC+ and skp in that order.  Note this assumes
# QLC+ has been started with the --web option.
#


QLC_PID=`ps -ef | grep qlcplus | grep web | tr -s ' ' | cut -f2 -d' '`


kill ${QLC_PID} # Kill QLC+
wait ${QLC_PID} # Wait for the program to terminate

# Now stop skp
sudo systemctl stop stagekitpied

exit 0
