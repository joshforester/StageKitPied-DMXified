#!/bin/bash

#
# Script for powering on DMX devices.  The below case utilizes a smart plug.
#


# This should be replaced by whatever logic you require to turn on your DMX devices
SMART_OUTLET_ON_URL1="http://192.168.1.251/rpc/Switch.Set?id=0&on=true"

curl -s "$SMART_OUTLET_ON_URL1" 2>&1

