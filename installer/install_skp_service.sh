#!/bin/bash

# TODO: install xbox pod guard
# TODO: install locate_usb.sh

SCRIPT_DIR=$(dirname "$(realpath "$0")")

USAGE="Usage: sudo $0 [SKP lights.ini File] [Mapping Config File] [QLC+ Workspace File]"
SKP_INSTALL_DIR=/opt/StageKitPied
SKP_INPUT_DIR=${SKP_INSTALL_DIR}/input
SKP_UDEV_DIR=${SKP_INSTALL_DIR}/udev
SKP_AUTOSTART_DIR=${SKP_INSTALL_DIR}/autostart
SKP_SERVICE_NAME=stagekitpied
UDEV_RULES_DIR=/etc/udev/rules.d
SYSTEMD_SERVICE_DIR=/lib/systemd/system
DEFAULT_SKP_LIGHTSINI_SOURCE_DIR=${SCRIPT_DIR}/../StageKitPied
DEFAULT_SKP_LIGHTSINI_SOURCE_FILE=lights.ini
DEFAULT_DMXIFIED_MAPPING_SOURCE_DIR=${SCRIPT_DIR}/../StageKitPied
DEFAULT_DMXIFIED_MAPPING_SOURCE_FILE=dmxified_mapping.xml
DEFAULT_QLCPLUS_WORKSPACE_SOURCE_DIR=${SCRIPT_DIR}
DEFAULT_QLCPLUS_WORKSPACE_SOURCE_FILE=qlcplusSkpDmx.qxw

# Determine autostart directory
USER_HOME=$(eval echo ~${SUDO_USER})
USER_AUTOSTART_DIR=${USER_HOME}/.config/autostart

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
elif [[ $# -eq 3 ]]; then
    # Check if the files exist
    file_exists "$1"
    file_exists "$2"
    file_exists "$3"

    # Parse each file to get PATH and FILE parts
    read -r SKP_LIGHTSINI_SOURCE_DIR SKP_LIGHTSINI_SOURCE_FILE <<< $(parse_path_and_file "$1")
    read -r DMXIFIED_MAPPING_SOURCE_DIR DMXIFIED_MAPPING_SOURCE_FILE <<< $(parse_path_and_file "$2")
    read -r QLCPLUS_WORKSPACE_SOURCE_DIR QLCPLUS_WORKSPACE_SOURCE_FILE <<< $(parse_path_and_file "$3")

    # Ensure SKP_LIGHTSINI_SOURCE_FILE is "lights.ini"
    if [[ "$SKP_LIGHTSINI_SOURCE_FILE" != "lights.ini" ]]; then
       echo "ERROR: The [SKP lights.ini File] must be 'lights.ini', but got '$SKP_LIGHTSINI_SOURCE_FILE'." 1>&2
       echo "${USAGE}" 1>&2
       exit 1
    fi

else
    # Incorrect number of arguments
    echo "ERROR: You must pass exactly 3 arguments or none." 1>&2
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
cp ${SCRIPT_DIR}/../StageKitPied/skp ${SKP_INSTALL_DIR}
cp ${SKP_LIGHTSINI_SOURCE_DIR}/${SKP_LIGHTSINI_SOURCE_FILE} ${SKP_INSTALL_DIR}
cp ${SCRIPT_DIR}/../StageKitPied/leds1.ini ${SKP_INSTALL_DIR}
cp ${SCRIPT_DIR}/../StageKitPied/leds2.ini ${SKP_INSTALL_DIR}
cp ${SCRIPT_DIR}/../StageKitPied/leds3.ini ${SKP_INSTALL_DIR}
cp ${SCRIPT_DIR}/../StageKitPied/leds4.ini ${SKP_INSTALL_DIR}
cp ${SCRIPT_DIR}/../StageKitPied/leds5.ini ${SKP_INSTALL_DIR}
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

# Setup udev to use perms to gatekeep the serial adapter from Qlcplus
# treating it like a DMX adapter with an FT232R chip.
mkdir -p ${SKP_UDEV_DIR}
UDEV_SERIALUSBGUARDRULE_SCRIPT=serialUsbGuard.sh
cp ${SCRIPT_DIR}/serialUsbGuard.sh ${SKP_UDEV_DIR}
chmod 770 ${SKP_UDEV_DIR}/${UDEV_SERIALUSBGUARDRULE_SCRIPT}

UDEV_SERIALUSBGUARDRULE_FILE=zzzz-99-serialUsbGuard.rules
cp ${SCRIPT_DIR}/${UDEV_SERIALUSBGUARDRULE_FILE} ${UDEV_RULES_DIR}
chmod 440 ${UDEV_RULES_DIR}/${UDEV_SERIALUSBGUARDRULE_FILE}
udevadm control --reload

echo "udev rule installed to prevent Qlcplus from claiming FT232R serial adapter."
echo ""

#####################################################

# Setup the service.
cp ${SCRIPT_DIR}/${SKP_SERVICE_NAME}.service ${SYSTEMD_SERVICE_DIR}
systemctl daemon-reload
#systemctl disable ${SKP_SERVICE_NAME}.service

#echo "Startup service installed and enabled during reboot.  It will be started at the end of this script."
echo "Systemd service installed.  It will be started at the end of this script."
echo ""

#####################################################

# Setup the QLC launcher
QLCPLUS_AUTOLAUNCHER_CONFIG=runqlc.desktop
TMP_FILE=/tmp/${QLCPLUS_AUTOLAUNCHER_CONFIG}
echo "[Desktop Entry]" > ${TMP_FILE}
echo "Type=Application" >> ${TMP_FILE}
echo "Exec=${SKP_AUTOSTART_DIR}/runQlc.sh ${SKP_AUTOSTART_DIR}/${QLCPLUS_WORKSPACE_SOURCE_FILE}" >> ${TMP_FILE}
mkdir -p ${USER_AUTOSTART_DIR}
mv ${TMP_FILE} ${USER_AUTOSTART_DIR}
chown ${SUDO_USER}:${SUDO_USER} ${USER_AUTOSTART_DIR}/${QLCPLUS_AUTOLAUNCHER_CONFIG}
chmod 440 ${USER_AUTOSTART_DIR}/${QLCPLUS_AUTOLAUNCHER_CONFIG}

QLCPLUS_AUTOLAUNCHER=runQlc.sh
mkdir -p ${SKP_AUTOSTART_DIR}
chown ${SUDO_USER}:${SUDO_USER} ${SKP_AUTOSTART_DIR}
cp ${SCRIPT_DIR}/${QLCPLUS_AUTOLAUNCHER} ${SKP_AUTOSTART_DIR}
chown ${SUDO_USER}:${SUDO_USER} ${SKP_AUTOSTART_DIR}/${QLCPLUS_AUTOLAUNCHER}
chmod 770 ${USER_AUTOSTART_DIR}/${QLCPLUS_AUTOLAUNCHER}

cp ${QLCPLUS_WORKSPACE_SOURCE_DIR}/${QLCPLUS_WORKSPACE_SOURCE_FILE} ${SKP_AUTOSTART_DIR}
chown ${SUDO_USER}:${SUDO_USER} ${SKP_AUTOSTART_DIR}/${QLCPLUS_WORKSPACE_SOURCE_FILE}
chmod 440 ${SKP_AUTOSTART_DIR}/${QLCPLUS_WORKSPACE_SOURCE_FILE}

echo "Qlcplus autostart scripts installed using ${QLCPLUS_WORKSPACE_SOURCE_DIR}/${QLCPLUS_WORKSPACE_SOURCE_FILE}."
echo "Please set your Raspberry Pi graphical desktop to autologin for user ${SUDO_USER}."
echo ""

#####################################################

echo "Starting ${SKP_SERVICE_NAME} service."
systemctl start ${SKP_SERVICE_NAME}

