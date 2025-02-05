#!/bin/bash

# Check if $1 is provided
if [ -z "$1" ]; then
    echo "Error: No workspace string provided."
    exit 1 
fi

QLCPLUS_WORKSPACE=$1


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

sleep 1

/usr/bin/qlcplus --open /home/josh/QlcplusOutputSimulatorDemo.qxw --web --operate &

sleep 5

kill `ps -ef | grep qlcplus | grep Demo | tr -s ' ' | cut -f2 -d' '`

sleep 1

sudo systemctl restart stagekitpied

sleep 5

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

sleep 1

/usr/bin/qlcplus --open /home/josh/QlcplusOutputSimulatorDemo.qxw --web --operate
