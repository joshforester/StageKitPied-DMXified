#!/usr/bin/bash

sleep 10

#/bin/udevadm settle

LOGFILE=/var/log/stagekitpied_serialUsbGuard.log

/usr/bin/date >> $LOGFILE
echo "Device plugged in $1" >> $LOGFILE

echo "Unix Permissions" >> $LOGFILE
echo `ls -l $1` >> $LOGFILE

# Log the ACL before setting it
echo "ACL:" >> $LOGFILE
getfacl "$1" >> $LOGFILE

# Restrict permissions
chown root:root "$1"
chmod 600 "$1"

# Remove ACLs
/usr/bin/setfacl -b "$1"

echo "Permissions set and ACLs removed for $1" >> $LOGFILE

echo "Unix Permissions" >> $LOGFILE
echo `ls -l $1` >> $LOGFILE

# Log the ACL after setting
echo "ACL:" >> $LOGFILE
getfacl "$1" >> $LOGFILE

