#!/usr/bin/env python3

#
# Script for reading GPIO-connected button and when pressed, alternating between
# running one script and a second script.  In this way, the button acts as a
# switch.  While this can be used for arbitrary commands, this script in this 
# package is intended to be used to start/stop the skp service with a physical 
# button.
#

import os
import sys
import subprocess
import argparse
import signal
from gpiozero import Button

# Set up argparse for argument parsing
parser = argparse.ArgumentParser(description="Monitor button press on specified GPIO pin and run different bash scripts on odd/even presses.")
parser.add_argument("gpio_pin", type=int, help="GPIO pin to monitor for button press (e.g., 17)")
parser.add_argument("bash_script_odd", type=str, help="Path to the bash script to execute on odd presses")
parser.add_argument("bash_script_even", type=str, help="Path to the bash script to execute on even presses")

# Parse arguments
args = parser.parse_args()

gpio_pin = args.gpio_pin
bash_script_odd = args.bash_script_odd
bash_script_even = args.bash_script_even

# Ensure both provided bash scripts exist and are executable
for script_path in [bash_script_odd, bash_script_even]:
    if not os.path.isfile(script_path):
        print(f"Error: The file '{script_path}' does not exist.")
        sys.exit(1)

    if not os.access(script_path, os.X_OK):
        print(f"Error: The file '{script_path}' is not executable.")
        sys.exit(1)

# Set up the button with the specified GPIO pin and debounce time (0.1 seconds)
button = Button(gpio_pin, bounce_time=0.1)

# Counter for button presses
press_count = 0

# Define a clean exit function
def clean_exit(signal, frame):
    sys.exit(0)

# Register signal handlers for SIGTERM and SIGINT
signal.signal(signal.SIGINT, clean_exit)   # Handle Ctrl+c (SIGINT)
signal.signal(signal.SIGTERM, clean_exit)  # Handle termination signal (SIGTERM)

# Wait for button press and execute the bash scripts
while True:
    button.wait_for_press()
    press_count += 1

    if press_count % 2 == 1:
        # Run the first script on odd presses
        print("THE BUTTON WAS PRESSED (Odd press)")
        subprocess.run([bash_script_odd], check=True)
    else:
        # Run the second script on even presses
        print("THE BUTTON WAS PRESSED (Even press)")
        subprocess.run([bash_script_even], check=True)

    button.wait_for_release()
    print("Released")
