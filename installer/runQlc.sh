#!/bin/bash

# Check if $1 is provided
if [ -z "$1" ]; then
    echo "Error: No workspace string provided."
    exit 1 
fi

QLCPLUS_WORKSPACE=$1

/usr/bin/qlcplus --open ${QLCPLUS_WORKSPACE} --web --operate &
