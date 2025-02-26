#!/usr/bin/env python3

import time
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
def create_file_for_button(button):
    try:
        # Get the pin number using the `pin` attribute directly (should work with LGPIOPin)
        pin_number = button.pin._number  # Use the internal `_number` attribute that holds the pin number
        
        if pin_number is None:
            raise ValueError("Unable to retrieve pin number from button.")
        
        # Format file name with the correct pin number
        file_name = f"button{pin_number:02d}.tmp"  # Zero-padded 2-digit pin number
        file_path = os.path.join(directory, file_name)
        
        # Create the file (touch it)
        with open(file_path, 'w'):
            pass
        print(f"Created {file_path}")
    except Exception as e:
        print(f"Error creating file for button: {e}")

# Function to remove the file corresponding to the button released
def remove_file_for_button(button):
    try:
        # Get the pin number using the `pin` attribute directly
        pin_number = button.pin._number  # Use the internal `_number` attribute that holds the pin number
        
        if pin_number is None:
            raise ValueError("Unable to retrieve pin number from button.")
        
        # Format file name with the correct pin number
        file_name = f"button{pin_number:02d}.tmp"  # Zero-padded 2-digit pin number
        file_path = os.path.join(directory, file_name)
        
        # Remove the file if it exists
        if os.path.exists(file_path):
            os.remove(file_path)
            print(f"Removed {file_path}")
    except Exception as e:
        print(f"Error removing file for button: {e}")

# Check if any files already exist before starting the button loop and remove them
for pin in args.button_numbers:
    file_name = f"button{pin:02d}.tmp"
    file_path = os.path.join(directory, file_name)
    if os.path.exists(file_path):
        os.remove(file_path)
        print(f"Removed existing file: {file_path}")

# Assign the press and release actions for each button
for button in buttons:
    button.when_pressed = lambda button=button: create_file_for_button(button)
    button.when_released = lambda button=button: remove_file_for_button(button)

# Main loop to keep the script running
while True:
    time.sleep(0.1)  # Main loop keeps running, waiting for button events
