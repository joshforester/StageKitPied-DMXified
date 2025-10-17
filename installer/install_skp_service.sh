#!/bin/bash

# TODO: podman installation, pull of image for QLC+, and setup
# TODO: install buttons scripts
# - locate_usb.sh
# - kill_skp.sh
# - run_skp.sh
# - skp_wrapper.sh
# - poweron/poweroff

INSTALLER_DIR=$(dirname "$(realpath "$0")")
INSTALLER_UDEV_DIR=${INSTALLER_DIR}/udev
INSTALLER_QLC_DIR=${INSTALLER_DIR}/qlc

USAGE="Usage: sudo $0 [SKP lights.ini File] [Mapping Config File] [QLC+ Workspace File] [QLC+ Fixtures Directory]"

SKP_INSTALL_DIR=/opt/StageKitPied
SKP_INPUT_DIR=${SKP_INSTALL_DIR}/input
SKP_UDEV_DIR=${SKP_INSTALL_DIR}/udev
SKP_QLC_DIR=${SKP_INSTALL_DIR}/qlc
SKP_QLC_FIXTURES_DIR=${SKP_QLC_DIR}/fixtures
SKP_SERVICE_NAME=stagekitpied
QLCPLUS_PODMAN_SERVICE_NAME=qlcplus-podman
UDEV_RULES_DIR=/etc/udev/rules.d
SYSTEMD_SERVICE_DIR=/lib/systemd/system
DEFAULT_SKP_LIGHTSINI_SOURCE_DIR=${INSTALLER_DIR}/../StageKitPied
DEFAULT_SKP_LIGHTSINI_SOURCE_FILE=lights.ini
DEFAULT_DMXIFIED_MAPPING_SOURCE_DIR=${INSTALLER_DIR}/../StageKitPied
DEFAULT_DMXIFIED_MAPPING_SOURCE_FILE=dmxified_mapping.xml
DEFAULT_QLCPLUS_WORKSPACE_SOURCE_DIR=${INSTALLER_DIR}
DEFAULT_QLCPLUS_WORKSPACE_SOURCE_FILE=qlcplusSkpDmx.qxw

# Check if the script is running as root, since service install requires it.
if [ "$EUID" -ne 0 ]; then
    echo "ERROR: This script must be run as root (use sudo):" 1>&2
    echo "${USAGE}" 1>&2
    exit 1
fi

# Function to check if a file exists
file_exists() {
    if [[ ! -f "$1" ]]; then
        echo "ERROR: $1 does not exist." 1>&2
        exit 1
    fi
}

# Function to check if a file exists
dir_exists() {
    if [[ ! -d "$1" ]]; then
        echo "ERROR: $1 does not exist." 1>&2
        exit 1
    fi
}

# Function to parse the path and file from an input
parse_path_and_file() {
    local input="$1"
    local path=""
    local file=""
    
    # Check if the input contains a path
    if [[ "$input" == */* ]]; then
        # Extract path and file from input
        path=$(dirname "$input")
        file=$(basename "$input")
    else
        # If no path, use current directory for path
        path="."
        file="$input"
    fi
    
    echo "$path" "$file"
}

# Check the number of arguments passed to the script
if [[ $# -eq 0 ]]; then
    # No arguments passed, use default values
    SKP_LIGHTSINI_SOURCE_DIR="$DEFAULT_SKP_LIGHTSINI_SOURCE_DIR"
    SKP_LIGHTSINI_SOURCE_FILE="$DEFAULT_SKP_LIGHTSINI_SOURCE_FILE"
    DMXIFIED_MAPPING_SOURCE_DIR="$DEFAULT_DMXIFIED_MAPPING_SOURCE_DIR"
    DMXIFIED_MAPPING_SOURCE_FILE="$DEFAULT_DMXIFIED_MAPPING_SOURCE_FILE"
    QLCPLUS_WORKSPACE_SOURCE_DIR="$DEFAULT_QLCPLUS_WORKSPACE_SOURCE_DIR"
    QLCPLUS_WORKSPACE_SOURCE_FILE="$DEFAULT_QLCPLUS_WORKSPACE_SOURCE_FILE"
    QLCPLUS_FIXTURES_SOURCE_DIR="$DEFAULT_QLCPLUS_FIXTURES_SOURCE_DIR" 
elif [[ $# -eq 4 ]]; then
    # Check if the files exist
    file_exists "$1"
    file_exists "$2"
    file_exists "$3"
    dir_exists "$4"

    # Parse each file to get PATH and FILE parts
    read -r SKP_LIGHTSINI_SOURCE_DIR SKP_LIGHTSINI_SOURCE_FILE <<< $(parse_path_and_file "$1")
    read -r DMXIFIED_MAPPING_SOURCE_DIR DMXIFIED_MAPPING_SOURCE_FILE <<< $(parse_path_and_file "$2")
    read -r QLCPLUS_WORKSPACE_SOURCE_DIR QLCPLUS_WORKSPACE_SOURCE_FILE <<< $(parse_path_and_file "$3")
    
    # Set dir to provided, existing one
    QLCPLUS_FIXTURES_SOURCE_DIR="$4"

    # Ensure SKP_LIGHTSINI_SOURCE_FILE is "lights.ini"
    if [[ "$SKP_LIGHTSINI_SOURCE_FILE" != "lights.ini" ]]; then
       echo "ERROR: The [SKP lights.ini File] must be 'lights.ini', but got '$SKP_LIGHTSINI_SOURCE_FILE'." 1>&2
       echo "${USAGE}" 1>&2
       exit 1
    fi

    # Ensure QLCPLUS_WORKSPACE_SOURCE_FILE is "qlcplusSkpDmx.qxw"
    if [[ "$QLCPLUS_WORKSPACE_SOURCE_FILE" != "qlcplusSkpDmx.qxw" ]]; then
       echo "ERROR: The [QLC+ Workspace File] must be 'qlcplusSkpDmx.qxw', but got '$QLCPLUS_WORKSPACE_SOURCE_FILE'." 1>&2
       echo "${USAGE}" 1>&2
       exit 1
    fi

else
    # Incorrect number of arguments
    echo "ERROR: You must pass exactly 4 arguments or none." 1>&2
    echo "${USAGE}" 1>&2
    exit 1
fi


#####################################################

echo "Stopping existing ${SKP_SERVICE_NAME} service."
echo ""
systemctl stop ${SKP_SERVICE_NAME}

#####################################################

# Setup the program in its place.
mkdir -p ${SKP_INSTALL_DIR}
cp ${INSTALLER_DIR}/../StageKitPied/skp ${SKP_INSTALL_DIR}
cp ${SKP_LIGHTSINI_SOURCE_DIR}/${SKP_LIGHTSINI_SOURCE_FILE} ${SKP_INSTALL_DIR}
cp ${INSTALLER_DIR}/../StageKitPied/leds1.ini ${SKP_INSTALL_DIR}
cp ${INSTALLER_DIR}/../StageKitPied/leds2.ini ${SKP_INSTALL_DIR}
cp ${INSTALLER_DIR}/../StageKitPied/leds3.ini ${SKP_INSTALL_DIR}
cp ${INSTALLER_DIR}/../StageKitPied/leds4.ini ${SKP_INSTALL_DIR}
cp ${INSTALLER_DIR}/../StageKitPied/leds5.ini ${SKP_INSTALL_DIR}
cp ${DMXIFIED_MAPPING_SOURCE_DIR}/${DMXIFIED_MAPPING_SOURCE_FILE} ${SKP_INSTALL_DIR}

chmod 770 ${SKP_INSTALL_DIR}/skp
chmod 440 ${SKP_INSTALL_DIR}/*ini
chmod 440 ${SKP_INSTALL_DIR}/${DMXIFIED_MAPPING_SOURCE_FILE}

echo "skp installed to ${SKP_INSTALL_DIR} with:"
echo "${SKP_LIGHTSINI_SOURCE_DIR}/${SKP_LIGHTSINI_SOURCE_FILE}"
echo "${DMXIFIED_MAPPING_SOURCE_DIR}/${DMXIFIED_MAPPING_SOURCE_FILE}"
echo ""

mkdir -p ${SKP_INPUT_DIR}
chown ${SUDO_USER}:${SUDO_USER} ${SKP_INPUT_DIR}

echo "Created ${SKP_INPUT_DIR} as a convenience for fileExistsInput elements in mapping file config."
echo ""

#####################################################

# Create symlinks for FT232R/ftdi chips so we can pass in a predictably-named DMX output device to the QLC+ container. 
mkdir -p ${SKP_UDEV_DIR}
UDEV_FT232RUSBRULE_FILE=zzzz-97-ft232rDeviceSymlinks.rules
cp ${INSTALLER_UDEV_DIR}/${UDEV_FT232RUSBRULE_FILE} ${UDEV_RULES_DIR}
chmod 440 ${UDEV_RULES_DIR}/${UDEV_FT232RUSBRULE_FILE}
udevadm control --reload

echo "udev rule installed predictably symlink FT232R/ftdi USB devices."
echo ""

#####################################################

# Setup udev to use perms to gatekeep the Xbox StageKit Light Pods from anything else
# because we are paranoid and only want skp to access them. 
mkdir -p ${SKP_UDEV_DIR}
UDEV_XBOXSTAGEKITPODGUARDRULE_SCRIPT=xboxStageKitPodGuard.sh
cp ${INSTALLER_UDEV_DIR}/${UDEV_XBOXSTAGEKITPODGUARDRULE_SCRIPT} ${SKP_UDEV_DIR}
chmod 770 ${SKP_UDEV_DIR}/${UDEV_XBOXSTAGEKITPODGUARDRULE_SCRIPT}

UDEV_XBOXSTAGEKITPODGUARDRULE_FILE=zzzz-98-xboxStageKitPodGuard.rules
cp ${INSTALLER_UDEV_DIR}/${UDEV_XBOXSTAGEKITGUARDRULE_FILE} ${UDEV_RULES_DIR}
chmod 440 ${UDEV_RULES_DIR}/${UDEV_XBOXSTAGEKITGUARDRULE_FILE}
udevadm control --reload

echo "udev rule installed to prevent anything but StageKitPied from claiming Xbox Light Pods."
echo ""

#####################################################

# Setup udev to use perms to gatekeep the serial adapter from Qlcplus
# treating it like a DMX adapter with an FT232R chip.
mkdir -p ${SKP_UDEV_DIR}
UDEV_SERIALUSBGUARDRULE_SCRIPT=serialUsbGuard.sh
cp ${INSTALLER_UDEV_DIR}/${UDEV_SERIALUSBGUARDRULE_SCRIPT} ${SKP_UDEV_DIR}
chmod 770 ${SKP_UDEV_DIR}/${UDEV_SERIALUSBGUARDRULE_SCRIPT}

UDEV_SERIALUSBGUARDRULE_FILE=zzzz-99-serialUsbGuard.rules
cp ${INSTALLER_UDEV_DIR}/${UDEV_SERIALUSBGUARDRULE_FILE} ${UDEV_RULES_DIR}
chmod 440 ${UDEV_RULES_DIR}/${UDEV_SERIALUSBGUARDRULE_FILE}
udevadm control --reload

echo "udev rule installed to prevent Qlcplus from claiming FT232R serial adapter."
echo ""

#####################################################

# Setup the QLC workspace

mkdir -p ${SKP_QLC_DIR}
chown ${SUDO_USER}:${SUDO_USER} ${SKP_QLC_DIR}

cp ${QLCPLUS_WORKSPACE_SOURCE_DIR}/${QLCPLUS_WORKSPACE_SOURCE_FILE} ${SKP_QLC_DIR}
chown ${SUDO_USER}:${SUDO_USER} ${SKP_QLC_DIR}/${QLCPLUS_WORKSPACE_SOURCE_FILE}
chmod 440 ${SKP_QLC_DIR}/${QLCPLUS_WORKSPACE_SOURCE_FILE}

echo "Qlcplus workspace installed using ${QLCPLUS_WORKSPACE_SOURCE_DIR}/${QLCPLUS_WORKSPACE_SOURCE_FILE} to ${SKP_QLC_DIR}."
echo ""

#####################################################

# Setup the QLC fixtures

mkdir -p ${SKP_QLC_DIR}
chown ${SUDO_USER}:${SUDO_USER} ${SKP_QLC_DIR}

cp -a ${QLCPLUS_FIXTURES_SOURCE_DIR} ${SKP_QLC_FIXTURES_DIR}
chown -R ${SUDO_USER}:${SUDO_USER} ${SKP_QLC_FIXTURES_DIR}
chmod -R 440 ${SKP_QLC_FIXTURES_DIR}

echo "Qlcplus fixtures installed using ${QLCPLUS_FIXTURES_SOURCE_DIR} to ${SKP_QLC_FIXTURES_DIR}."
echo ""

#####################################################

# Setup the service.
cp ${INSTALLER_DIR}/${QLCPLUS_PODMAN_SERVICE_NAME}.service ${SYSTEMD_SERVICE_DIR}
systemctl daemon-reload

echo "Systemd service for QLC+ Podman Container installed.  It will be started at the end of this script."
echo ""

#####################################################

# Setup the service.
cp ${INSTALLER_DIR}/${SKP_SERVICE_NAME}.service ${SYSTEMD_SERVICE_DIR}
systemctl daemon-reload
#systemctl disable ${SKP_SERVICE_NAME}.service

echo "Systemd service for StageKitPied installed.  It will be started at the end of this script."
echo ""

#####################################################

echo "Starting ${QLCPLUS_PODMAN_SERVICE_NAME} service."
systemctl start ${QLCPLUS_PODMAN_SERVICE_NAME}

#####################################################

echo "Starting ${SKP_SERVICE_NAME} service."
systemctl start ${SKP_SERVICE_NAME}

