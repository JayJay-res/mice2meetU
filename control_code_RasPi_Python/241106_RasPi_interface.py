# Import required libraries
from gpiozero import DigitalOutputDevice
from gpiozero import DigitalInputDevice
import time
from datetime import datetime

# Define durations for various stages in milliseconds
duration_social = 1000              # Duration for the social interaction between two mice
duration_slidedown = 2000           # Safety margin for the mice to slide down to the box
                                            # before machine proceeds further
duration_run_cycle_interval = 5000  # Interval before the next social interaction cycle begins

# Initialize output GPIO pins
tilt = DigitalOutputDevice(20) # BNC for tilt control signal
door = DigitalOutputDevice(21) # BNC for door control signal

# Initialize input GPIO pins the proximity sensor as a digital input device
sensor_prox = DigitalInputDevice(12)     # Digital input from proximity sensor on GPIO pin 12



# Initialization: Turn off both tilt and door controls
tilt.off()  # Ensure tilt is in the initial (off) position
door.off()  # Ensure door is in the initial (off) position
MODE = 1    # Start the machine in MODE 1
print("Now entering MODE", MODE)



# Main program loop
while True:
    if MODE == 1:   # Start stage
        # Tilt the box low to prepare for interaction
        tilt.on()

        # Transition to the next MODE
        MODE = 2
        print("Now entering MODE", MODE)



    if MODE == 2:   # Pre-meeting stage
        # Wait for the proximity sensor to detect an object, then open the door
        if sensor_prox.value == 1:
            door.on()

            # Transition to the next MODE
            MODE = 3
            print("Now entering MODE", MODE)
            interaction_timer = int(round(time.time() * 1000))  # Record the start time MODE 3



    if MODE == 3:   # Meeting stage
        # Maintain the door opened for a set interaction duration, then tilt the box high
        if (int(round(time.time() * 1000)) - interaction_timer) > duration_social:
            tilt.off()

            # Transition to the next MODE
            MODE = 4
            print("Now entering MODE", MODE)



    if MODE == 4:   # Return object to box stage
        # Wait for the proximity sensor to no longer detect an object, then close the door
        if sensor_prox.value == 0:
            door.off()

            # Transition to the next MODE
            MODE = 5
            print("Now entering MODE", MODE)
            slide_down_timer = int(round(time.time() * 1000))  # Record the start time of MODE 5



    if MODE == 5:
        # Provide time for the object to slide back into the box
        # Maintain the box tilted high until the timer ends, then tilt the box low
        if (int(round(time.time() * 1000)) - slide_down_timer) > duration_slidedown:
            tilt.on()

            # Transition to the next mode
            MODE = 6
            print("Now entering MODE", MODE)
            interval_timer = int(round(time.time() * 1000))  # Record the start time of MODE 6



    if MODE == 6:
        # Wait for the social interaction interval timer to elapse, resetting the machine cycle
        if (int(round(time.time() * 1000)) - interval_timer) > duration_run_cycle_interval:
            MODE = 1 # Reset to MODE 1 to begin a new cycle
            print("Now entering MODE", MODE)
