
# to import the RPi.GPIO module
# by doing it this way, you can refer to it as just GPIO through the rest of your script
import RPi.GPIO as GPIO

#library to add delay
import time

# instantiate GPIO as an object
GPIO.setmode(GPIO.BCM)

# define GPIO pin 
doorPin = 3

# setup channel [GPIO.setup(channel, GPIO.IN/OUT]
GPIO.setup(doorPin, GPIO.IN)

# create the function to print door status
def doorStatus():
    if (GPIO.input(doorPin))==1:
        print("Door is open")
    else :
        print ("Door is closed")

# provide a loop to display door status
while True:
    doorStatus()
GPIO.cleanup()
