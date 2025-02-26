#!/usr/bin/env python3

#
# Script for reading GPIO-connected button and when pressed, touches a file named
# after the GPIO pin to which the button is connected; when pressed a second time, 
# deletes the file.  This script is intended to be used in tandem with a 
# <fileExistsInput> mapping file element.
#

import time
import os
import argparse
import signal
import sys
from gpiozero import Button

# Set up argument parsing
parser = argparse.ArgumentParser(description='Monitor multiple button presses on GPIO pins and toggle corresponding files')

# Add arguments for directory and button numbers
parser.add_argument('directory', type=str, help='The directory where the button files will be created')
parser.add_argument('button_numbers', type=int, nargs='+', help='List of GPIO pin numbers connected to buttons')

# Parse the command-line arguments
args = parser.parse_args()

# Set up the directory and create it if it doesn't exist
directory = args.directory
if not os.path.exists(directory):
    os.makedirs(directory)

# Set up the buttons with debounce time
debounce_time = 0.1  # Debounce time in seconds
buttons = [Button(pin, bounce_time=debounce_time) for pin in args.button_numbers]

# Dictionary to keep track of press counts for each button
press_count = {pin: 0 for pin in args.button_numbers}  # Ensure all button numbers are initialized

# Function to handle graceful exit
def clean_exit(signal, frame):
    sys.exit(0)

# Register signal handlers
signal.signal(signal.SIGINT, clean_exit)  # Handle Ctrl+C (SIGINT)
signal.signal(signal.SIGTERM, clean_exit)  # Handle termination signal (SIGTERM)

# Function to create the file corresponding to the button pressed
def create_file_for_button(pin):
    # Extract the GPIO pin number correctly from the button object
    pin_number = pin.pin.number  # Access the 'number' attribute (the actual GPIO pin number)

    # Format file name with the correct pin number
    file_name = f"button{pin_number:02d}.tmp"  # Zero-padded 2-digit pin number
    file_path = os.path.join(directory, file_name)
    with open(file_path, 'w'):
        pass
    print(f"Created {file_path}")

# Function to remove the file corresponding to the button pressed
def remove_file_for_button(pin):
    # Extract the GPIO pin number correctly from the button object
    pin_number = pin.pin.number  # Access the 'number' attribute (the actual GPIO pin number)

    # Format file name with the correct pin number
    file_name = f"button{pin_number:02d}.tmp"  # Zero-padded 2-digit pin number
    file_path = os.path.join(directory, file_name)
    if os.path.exists(file_path):
        os.remove(file_path)
        print(f"Removed {file_path}")

# Function to toggle file creation/removal based on press count
def toggle_file_for_button(button):
    pin = button  # button is the actual Button object here, not just the pin number

    # Ensure press count is initialized for this pin
    if pin.pin.number not in press_count:
        press_count[pin.pin.number] = 0  # Initialize pin if it doesn't exist

    press_count[pin.pin.number] += 1
    if press_count[pin.pin.number] % 2 == 1:  # Odd press count - create the file
        create_file_for_button(pin)
    else:  # Even press count - remove the file
        remove_file_for_button(pin)

# Check if any files already exist before starting the button loop and remove them
for pin in args.button_numbers:
    file_name = f"button{pin:02d}.tmp"
    file_path = os.path.join(directory, file_name)
    if os.path.exists(file_path):
        os.remove(file_path)
        print(f"Removed existing file: {file_path}")

# Assign the press action for each button directly
for button in buttons:
    button.when_pressed = lambda button=button: toggle_file_for_button(button)

# Main loop to keep the script running
while True:
    time.sleep(0.1)  # Main loop keeps running, waiting for button events

