#!/bin/bash
set -e

DISPLAY_NUM=:1
VNC_PORT=5901
VNC_PASSWD_FILE=/root/.vnc/passwd

# Clean up old locks (VNC/X crashes leave these behind)
rm -f /tmp/.X1-lock
rm -rf /tmp/.X11-unix/X1

touch /root/.Xauthority
chown root:root /root/.Xauthority
chmod 600 /root/.Xauthority

# Set VNC password if not already set
if [ ! -f "$VNC_PASSWD_FILE" ]; then
    mkdir -p /root/.vnc
    echo "${VNC_PASSWORD:-qlcplus}" | vncpasswd -f > "$VNC_PASSWD_FILE"
    chmod 600 "$VNC_PASSWD_FILE"
fi

# Start VNC server (will launch Openbox via xstartup)
vncserver "$DISPLAY_NUM" -geometry 1920x1080 -depth 24 \
          -SecurityTypes VncAuth -passwd "$VNC_PASSWD_FILE" -localhost=no

# Wait until VNC server is listening
echo "Waiting for VNC server to start..."
MAX_WAIT=15
WAITED=0
while ! netstat -tln | grep -q ":$VNC_PORT"; do
    sleep 1
    WAITED=$((WAITED + 1))
    if [ $WAITED -ge $MAX_WAIT ]; then
        echo "VNC server did not start within $MAX_WAIT seconds"
        exit 1
    fi
done
echo "VNC server is listening on port $VNC_PORT"

# Export display
export DISPLAY=$DISPLAY_NUM

# Wait until X server is ready
echo "Waiting for X server to be ready..."
MAX_WAIT=15
WAITED=0
while ! xdpyinfo -display "$DISPLAY_NUM" >/dev/null 2>&1; do
    sleep 1
    WAITED=$((WAITED + 1))
    if [ $WAITED -ge $MAX_WAIT ]; then
        echo "X server did not start within $MAX_WAIT seconds"
        exit 1
    fi
done
echo "X server is ready, launching QLC+"

# Launch QLC+ in foreground
qlcplus --web --open /root/.qlcplus/projects/qlcplusSkpDmx2.qxw --operate

