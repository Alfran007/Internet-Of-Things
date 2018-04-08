
# to import the RPi.GPIO module
# by doing it this way, you can refer to it as just GPIO through the rest of your script
import RPi.GPIO as GPIO

# library to add delay
import time
# instantiate GPIO as an object
GPIO.setmode(GPIO.BCM)

# define GPIO pin 
powerPin = 4

# setup channel [GPIO.setup(channel, GPIO.IN/OUT]
GPIO.setup(powerPin, GPIO.IN)

# create the function to print power button status
def powerStatus():
    if (GPIO.input(powerPin))==1:
        print("Power Button is ON")
    else :
        print ("Power Button is OFF")

# provide a loop to display power status
while True:
    powerStatus()
    time.sleep(2)
GPIO.cleanup()
