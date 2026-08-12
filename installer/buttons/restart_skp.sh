#!/bin/bash

#
# Wrapper script for bouncing JUST skp in a defensive way.
#

SKPD_DIR=/opt/StageKitPied
SKPD_OPTIONAL_DIR=${SKPD_DIR}/optional

sudo systemctl stop stagekitpied
${SKPD_OPTIONAL_DIR}/skp_wrapper.sh

exit 0

