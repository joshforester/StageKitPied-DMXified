#!/bin/bash

#
# Script for powering on DMX devices.  The below case utilizes some smart plugs.
#


# This should be replaced by whatever logic you require to turn on your DMX devices
SMART_OUTLET_ON_URL1="http://192.168.1.251/rpc/Switch.Set?id=0&on=true"
SMART_OUTLET_ON_URL2="http://192.168.1.252/rpc/Switch.Set?id=0&on=true"
SMART_OUTLET_ON_URL3="http://192.168.1.253/rpc/Switch.Set?id=0&on=true"

curl -s "$SMART_OUTLET_ON_URL1" 2>&1
curl -s "$SMART_OUTLET_ON_URL2" 2>&1
curl -s "$SMART_OUTLET_ON_URL3" 2>&1

echo ""

exit 0
