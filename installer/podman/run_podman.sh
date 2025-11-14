#!/bin/bash

/opt/StageKitPied/podman/destroy_podman.sh

echo "Launching QLC+ container."

#
#  -e VNC_PASSWORD=qlcplus # The TigerVNC VNC Viewer client connection password (see below)
#  -p 5901:5901 # Host 5901 -> Container VNC :1 - TigerVNC VNC Viewer client can connect here to view QLC+ live (VNC server 192.168.1.x:5901, where 192.168.1.x is your RPi's local IP address)
#  -p 10000:9999 # Host 10000 -> QLC+ :9999 - WebSocket for receiving instructions from StageKitPied-DMXified.  Also, RPi's port 10000 for QLC+ Virtual Console web interface (http://192.168.1.x:10000).
#  --mount type=tmpfs,target=/dev/bus/usb # provides an empty bus to keep container's hands off of other usb devices
#  -v /opt/StageKitPied/autostart/fixtures # QLC+'s fixtures for your local lighting setup
#  --device /dev/ftdi-A10JGB3H # your DMX adapter that QLC will use to send out DMX instructions to lighting devices
#
podman run -d \
  --privileged \
  --name qlcplus-vnc-openbox \
  -e VNC_PASSWORD=qlcplus \
  -p 5901:5901 \
  -p 10000:9999 \
  -v /opt/StageKitPied/qlc:/root/.qlcplus/projects \
  -v /opt/StageKitPied/qlc/fixtures:/root/.qlcplus/fixtures \
  --device /dev/ftdi-A10JGB3H:/dev/ftdi-A10JGB3H \
  -v /dev/serial:/dev/serial \
  -v /dev/bus/usb:/dev/bus/usb \
  --log-level=debug \
  localhost/qlcplus-vnc-openbox

echo "Launching QLC+ container complete."
