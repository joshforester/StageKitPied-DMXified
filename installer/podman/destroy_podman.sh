#!/bin/bash

CONTAINER_NAME=qlcplus-vnc-openbox

echo "Destroying QLC+ container."

podman stop ${CONTAINER_NAME}
podman rm ${CONTAINER_NAME}

echo "Destroying QLC+ container complete."

