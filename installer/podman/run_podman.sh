#!/bin/bash

SKPD_DIR=/opt/StageKitPied
SKPD_QLC_DIR=/opt/StageKitPied
SKPD_PODMAN_DIR=${SKPD_DIR}/podman

USB_TO_DMX_ADAPTER_SERIAL=A10JGB3H
CONTAINER_NAME=qlcplus-vnc-openbox

HOST_IP=$(ip route get 1.1.1.1 2>/dev/null | awk '{print $7; exit}')
VNC_PORT=5901
QLC_PORT=10000

${SKPD_PODMAN_DIR}/destroy_podman.sh

echo "Launching QLC+ container."

#
#  -e VNC_PASSWORD=qlcplus # The TigerVNC VNC Viewer client connection password (see below)
#  -p 5901:5901 # Host 5901 -> Container VNC :1 - TigerVNC VNC Viewer client can connect here to view QLC+ live (VNC
#               # server 192.168.1.x:5901, where 192.168.1.x is your RPi's local IP address)
#  -p 10000:9999 # Host 10000 -> QLC+ :9999 - WebSocket for receiving instructions from StageKitPied-DMXified.  Also,
#                # RPi's port 10000 for QLC+ Virtual Console web interface (http://192.168.1.x:10000).
#  --mount type=tmpfs,target=/dev/bus/usb # provides an empty bus to keep container's hands off of other usb devices
#  -v /opt/StageKitPied/autostart/fixtures # QLC+'s fixtures for your local lighting setup
#  --device /dev/ftdi-A10JGB3H # your DMX adapter that QLC will use to send out DMX instructions to lighting devices
#
podman run -d \
  --privileged \
  --name ${CONTAINER_NAME} \
  -e VNC_PASSWORD=qlcplus \
  -p ${VNC_PORT}:${VNC_PORT} \
  -p ${QLC_PORT}:9999 \
  -v ${SKPD_QLC_DIR}:/root/.qlcplus/projects \
  -v ${SKPD_QLC_DIR}/fixtures:/root/.qlcplus/fixtures \
  --device /dev/ftdi-${USB_TO_DMX_ADAPTER_SERIAL}:/dev/ftdi-${USB_TO_DMX_ADAPTER_SERIAL} \
  -v /dev/serial:/dev/serial \
  -v /dev/bus/usb:/dev/bus/usb \
  --log-level=debug \
  localhost/${CONTAINER_NAME}

echo "Launching QLC+ container complete."
echo "VNC  : tcp/${VNC_PORT}"
echo "WebUI: http://${HOST_IP}:${QLC_PORT}"

exit 0
