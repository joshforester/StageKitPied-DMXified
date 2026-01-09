#!/bin/bash

set -e

CONTAINER_NAME="qlcplus-vnc-openbox"
IMAGE="ghcr.io/joshforester/stagekitpied-dmxified/${CONTAINER_NAME}:latest"

echo "==== StageKitPied Podman Installer ===="

# ---------------------------
# Install Podman if missing
# ---------------------------
if ! command -v podman >/dev/null 2>&1; then
    echo "Podman not found. Installing..."
    sudo apt-get update
    sudo apt-get install -y podman
else
    echo "Podman already installed."
fi

# ---------------------------
# Stop & remove existing container
# ---------------------------
if podman container exists "$CONTAINER_NAME"; then
    echo "Stopping existing container..."
    podman stop "$CONTAINER_NAME" || true
    podman rm "$CONTAINER_NAME"
fi

# ---------------------------
# Pull latest image
# ---------------------------
echo "Pulling latest image from GHCR..."
podman pull "$IMAGE"

exit 0
