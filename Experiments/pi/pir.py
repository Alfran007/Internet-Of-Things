
# to import the RPi.GPIO module
# by doing it this way, you can refer to it as just GPIO through the rest of your script
import RPi.GPIO as GPIO

# library to add delay
import time

# instantiate GPIO as an object
GPIO.setmode(GPIO.BCM)

# define GPIO pin 
motionPin = 2

# setup channel [GPIO.setup(channel, GPIO.IN/OUT]
GPIO.setup(motionPin, GPIO.IN)

# create the function to print motion status
def motionStatus():
    if (GPIO.input(motionPin))==1:
        print("Motion is Detected")
    else :
        print ("Motion is not Detected")

# provide a loop to display motion status
while True:
    motionStatus()
    time.sleep(2)
GPIO.cleanup()
