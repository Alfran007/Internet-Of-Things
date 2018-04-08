# include RPi libraries in to Python code
import RPi.GPIO as GPIO
import time
import math

# define GPIO pins with variables a_pin and b_pin
mpin = 17
tpin = 27

# instantiate GPIO as an object
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
cap=0.000001
adj=2.130620985
i=0
t=0 
while True:
    GPIO.setup(mpin, GPIO.OUT)
    GPIO.setup(tpin, GPIO.OUT)
    GPIO.output(mpin, False)
    GPIO.output(tpin, False)
    time.sleep(0.2)
    GPIO.setup(mpin, GPIO.IN)
    time.sleep(0.2)
    GPIO.output(tpin, True)
    starttime=time.time()
    while GPIO.input(mpin) == GPIO.LOW:
        endtime=time.time()
    measureresistance=endtime-starttime

    res=(measureresistance/cap)*adj
    i=i+1
    t=t+res
    if i==10:
           t=t/i
           print(float(5e9) * (math.log10(t)**-12.78))
           i=0
           t=0
           

