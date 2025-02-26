#!/bin/bash

#
# Script for powering off DMX devices.  The below case utilizes a smart plug.
#


# This should be replaced by whatever logic you require to turn off your DMX devices
SMART_OUTLET_OFF_URL1="http://192.168.1.251/rpc/Switch.Set?id=0&on=false"

curl -s "$SMART_OUTLET_OFF_URL1" 2>&1

