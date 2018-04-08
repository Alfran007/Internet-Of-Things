# Import all the libraries we need to run
import sys
#import RPi.GPIO as GPIO
import os
from time import sleep
#import Adafruit_DHT
import urllib2


#Setup our API and delay
myAPI = "YV4G670RMOI5XPXS" # Thingspeak API key
myDelay = 1 #how many seconds between posting data

#GPIO.setmode(GPIO.BCM)
#GPIO.setup(RCpin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)



def getSensorData():
    #RHW, TW = Adafruit_DHT.read_retry(Adafruit_DHT.DHT11, DHTpin)
    RHW, TW = (39.6, 31.1) # RHW = Humidity, TW = Temp-C
    
    #Convert from Celius to Farenheit
    TWF = 9/5*TW+32 # TWF = Temp-F
   
    # return dict
    return (str(RHW), str(TW),str(TWF))

# main() function
def main():
    
    print 'starting...'

    baseURL = 'https://api.thingspeak.com/update?api_key=%s' % myAPI
    print baseURL
    
    while True:
        try:
            RHW, TW, TWF = getSensorData()

            f = urllib2.urlopen(baseURL + 
                                "&field1=%s&field2=%s&field3=%s" % (TW, TWF, RHW))
            print TW + " " + TWF+ " " + RHW 
            f.close()
            

            sleep(int(myDelay))
        except:
            print 'exiting.'
            break
main()
