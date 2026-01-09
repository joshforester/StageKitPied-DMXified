#!/bin/bash

TARGET_SERIAL="A10JGB3H"

# Try via ttyUSB device first
for dev in /sys/bus/usb-serial/devices/ttyUSB*; do
    [[ -f "$dev/device/serial" ]] || continue
    serial=$(cat "$dev/device/serial")
    if [[ "$serial" == "$TARGET_SERIAL" ]]; then
        usbdev=$(readlink -f "$dev/device" | sed 's|/usb-serial/.*||')
        echo "Resetting FTDI (ttyUSB) $serial at $usbdev"
        echo 0 | sudo tee "$usbdev/authorized" >/dev/null
        sleep 1
        echo 1 | sudo tee "$usbdev/authorized" >/dev/null
        sudo udevadm trigger --subsystem-match=tty
        sudo udevadm settle --timeout=10
        exit 0
    fi
done

# If not found, fall back to raw USB devices
for dev in /sys/bus/usb/devices/*; do
    [[ -f "$dev/idVendor" && -f "$dev/idProduct" && -f "$dev/serial" ]] || continue
    vendor=$(cat "$dev/idVendor")
    product=$(cat "$dev/idProduct")
    serial=$(cat "$dev/serial")

    # The following vendor and product IDs are for the serial adapter (but also the same is used for my USB to DMX adapter):
    # Future Technology Devices International, Ltd FT232 Serial (UART) IC
    if [[ "$vendor" == "0403" && "$product" == "6001" && "$serial" == "$TARGET_SERIAL" ]]; then
        echo "Resetting FTDI (USB) $serial at $dev"
        echo 0 | sudo tee "$dev/authorized" >/dev/null
        sleep 1
        echo 1 | sudo tee "$dev/authorized" >/dev/null
        sudo udevadm trigger --subsystem-match=tty
        sudo udevadm settle --timeout=10
        exit 0
    fi
done

echo "Device with serial $TARGET_SERIAL not found."
exit 1

