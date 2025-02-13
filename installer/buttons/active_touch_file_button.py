#!/usr/bin/env python3

#
# Script for reading GPIO-connected button and when pressed, touches a file;
# when released, deletes a file.  This script is intended to be used in tandem
# with a <fileExistsInput> mapping file element.
#

import os
import argparse
import signal
import sys
from gpiozero import Button

# Set up argument parsing
parser = argparse.ArgumentParser(description='Monitor a button press on a GPIO pin and create/remove a file')

# Add arguments for button number and file path
parser.add_argument('button_number', type=int, help='The GPIO pin number connected to the button')
parser.add_argument('file_path', type=str, help='The full path to the file to create and remove on button press')

# Parse the command-line arguments
args = parser.parse_args()

# Set up the button using the GPIO pin number passed in as a parameter with debounce time
debounce_time = 0.2  # Debounce time in seconds
button = Button(args.button_number, bounce_time=debounce_time)
file_path = args.file_path

# Function to handle graceful exit
def clean_exit(signal, frame):
    sys.exit(0)

# Register signal handlers
signal.signal(signal.SIGINT, clean_exit)  # Handle Ctrl+C (SIGINT)
signal.signal(signal.SIGTERM, clean_exit)  # Handle termination signal (SIGTERM)

# Main loop to monitor button presses
while True:
    button.wait_for_press()
    print("THE BUTTON WAS PRESSED")
    
    # Create or touch the file
    with open(file_path, 'w'):
        pass
    
    button.wait_for_release()
    print("Released")
    
    # Remove the file
    os.remove(file_path)

