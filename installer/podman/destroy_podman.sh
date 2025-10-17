#!/bin/bash

echo "Destroying QLC+ container."

podman stop qlcplus-vnc-openbox
podman rm qlcplus-vnc-openbox

echo "Destroying QLC+ container complete."

