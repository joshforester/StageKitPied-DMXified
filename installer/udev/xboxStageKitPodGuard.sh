#!/usr/bin/bash

LOGFILE=/var/log/stagekitpied_xboxStagekitPodGuard.log

DEVICE_PATH=$1


echo "${DEVICE_PATH}: $(/usr/bin/date) Device plugged in." >> $LOGFILE

echo "${DEVICE_PATH}: Unix Permissions" >> $LOGFILE
echo "${DEVICE_PATH}: $(ls -l "${DEVICE_PATH}")" >> $LOGFILE

# Log the ACL before setting it
echo "${DEVICE_PATH}: ACL:" >> $LOGFILE
echo "${DEVICE_PATH}: $(getfacl -p "${DEVICE_PATH}")" >> $LOGFILE

# Restrict permissions
chown root:root "${DEVICE_PATH}"
chmod 600 "${DEVICE_PATH}"

# Remove ACLs
/usr/bin/setfacl -b "${DEVICE_PATH}"

echo "${DEVICE_PATH}: Permissions set and ACLs removed." >> $LOGFILE

echo "${DEVICE_PATH}: Unix Permissions" >> $LOGFILE
echo "${DEVICE_PATH}: $(ls -l "${DEVICE_PATH}")" >> $LOGFILE

# Log the ACL after setting
echo "${DEVICE_PATH}: ACL:" >> $LOGFILE
echo "${DEVICE_PATH}: $(getfacl -p "${DEVICE_PATH}")" >> $LOGFILE

