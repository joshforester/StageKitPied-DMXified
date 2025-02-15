#!/usr/bin/env python3

#
# Script for reading GPIO-connected button and when pressed, touches a file named
# after the GPIO pin to which the button is connected; when released, deletes the
# file.  This script is intended to be used in tandem with a <fileExistsInput> 
# mapping file element.
#

import os
import argparse
import signal
import sys
from gpiozero import Button

# Set up argument parsing
parser = argparse.ArgumentParser(description='Monitor multiple button presses on GPIO pins and create/remove corresponding files')

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

# Function to handle graceful exit
def clean_exit(signal, frame):
    sys.exit(0)

# Register signal handlers
signal.signal(signal.SIGINT, clean_exit)   # Handle Ctrl+C (SIGINT)
signal.signal(signal.SIGTERM, clean_exit)  # Handle termination signal (SIGTERM)

# Function to create the file corresponding to the button pressed
def create_file_for_button(pin):
    file_name = f"button{pin:02d}.tmp"     # Zero-padded 2-digit pin number
    file_path = os.path.join(directory, file_name)
    with open(file_path, 'w'):
        pass
    print(f"Created {file_path}")

# Function to remove the file corresponding to the button released
def remove_file_for_button(pin):
    file_name = f"button{pin:02d}.tmp"     # Zero-padded 2-digit pin number
    file_path = os.path.join(directory, file_name)
    if os.path.exists(file_path):
        os.remove(file_path)
        print(f"Removed {file_path}")

# Check if any files already exist before starting the button loop and remove them
for pin in args.button_numbers:
    file_name = f"button{pin:02d}.tmp"
    file_path = os.path.join(directory, file_name)
    if os.path.exists(file_path):
        os.remove(file_path)
        print(f"Removed existing file: {file_path}")

# Assign the press and release actions for each button
for button in buttons:
    button.when_pressed = lambda pin=button.pin: create_file_for_button(pin)
    button.when_released = lambda pin=button.pin: remove_file_for_button(pin)

# Main loop to keep the script running
try:
    while True:
        pass  # Main loop keeps running, waiting for button events
