#  -p 5901:5901    # Host 5901 → Container VNC :1
#  -p 10000:10000  # Host 10000 → QLC+ WebSocket
#  --mount type=tmpfs,target=/dev/bus/usb # provides an empty bus to keep container's hands off of other usb devices
podman run -d \
  --privileged \
  --name qlcplus-vnc-openbox \
  -e VNC_PASSWORD=qlcplus \
  -p 5901:5901 \
  -p 10000:9999 \
  -v /opt/StageKitPied/autostart:/root/.qlcplus/projects \
  -v /home/josh/.qlcplus/fixtures:/root/.qlcplus/fixtures \
  --device /dev/ftdi-A10JGB3H:/dev/ftdi-A10JGB3H \
  -v /dev/serial:/dev/serial \
  -v /dev/bus/usb:/dev/bus/usb \
  --log-level=debug \
  localhost/qlcplus-vnc-openbox

