# -*- coding: utf-8 -*-
"""
Created on Thu Mar 08 18:51:49 2018

@author: Kunal
"""

# Required Libraries
import json
import time
import urllib2
import requests
import paho.mqtt.client as mqtt


# Name of the MQTT Broker
Broker = "demo.thingsboard.io"

# Receive messages on this topic
sub_topic = "v1/devices/me/attributes/response/+"

# Access Token for Thingsboard Authentication
Acces_token = "kw7lALkNnvcpri3OStkG"

# Api Key for MLab Database
Api_Key = "Lfs9n3Dfjn0Hydr2bqf5NtJHHIs2tDdv"

# Base URL for MLab API
base_url = "https://api.mlab.com/api/1"

# Credintials for MLab Database
my_db = "thingsboard"    # Database Name
my_collection = "Data"   # Collection Name

sms_ph_number = "9928492120"    # Number to send SMS alert to

# range for Temperature 
temp_uppr_limit = 40
temp_lowr_limit = 5


############## Data To MLab Section ##############

def data_to_MLab(data):
    ''' Inserting Data to MLab Database '''
    
    try:    
        # Creating MLab URL with Database name ,Collection name and API_Key
        url = base_url+"/databases/"+my_db+"/collections/"+my_collection+"?apiKey="+Api_Key
        
        # Sending Data as JSON to MLab using POST request
        res = requests.post(url,json=data)
        response = json.loads(res.text)  # Getting the MLab Response
        
        if response.has_key('message'):
            return response    # Printing the MLab Response
        else:
            return  "Data Added Successfully"
        
    except Exception as e:
        return "ERROR :",str(e)  # Returns Error if any
    

############## SMS Section ##############

def sms(number,temp): 
    ''' Setting up SMS service i.e. Credintials, message-body etc '''

    # Alert Message to be sent
    message = "SMS ALERT Room Temperature is {0}C".format(temp)
    message = "+".join(message.split(' '))

    # Adding number and message to SMS url.(P.S. rest all the attributes remains same as per service provider)
    url ='http://sms.dataoxytech.com/index.php/smsapi/httpapi/?uname=sylvester007&password=forskmnit&sender=FORSKT&receiver={0}&route=TA&msgtype=1&sms={1}'.format(number,message)

    try:
        send_sms = urllib2.urlopen(url)  # Hitting the SMS url
        return send_sms.read() # Returns SMS Response
    
    except Exception as e:
        return "Error:",str(e)


############## MQTT section ##################

def on_connect(client, userdata, rc, *extra_params):
    ''' The callback for when the client receives a CONNACK response from the server.'''
    
    print('Connected with result code '+str(rc))
    
    # Creating a blank publish request for MQTT Broker with QoS 1, in order to Subscribe Data
    client.publish('v1/devices/me/attributes/request/1', "{}", 1)
    
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe(sub_topic)


def on_message(client, userdata, msg):
    ''' The callback for when a PUBLISH message is received from the server.'''
    
    print("Message received on topic "+msg.topic+" with QoS "+str(msg.qos)+" and payload "+msg.payload)
    
    # Getting Data from the msg Payload
    user = json.loads(msg.payload)
    user_data = user["client"]
    
    # Adding Timestamp Field in the user data
    user_data["Timestamp"] = time.time()
    
    print data_to_MLab(user_data)   # Adding Data to MLab Database
    
    # Geting Temperature and checking, if it is in between the threshold values
    temp = float(user_data["temperature"])
    
    if not(temp_lowr_limit < temp < temp_uppr_limit):
        print sms(sms_ph_number, temp)   # Send SMS if Temp not in between the threshold values
    else:
        print "Temperature is under control"


################## MQTT Registeration section ##################
        
mqttclient = mqtt.Client()   # Creating object for MQTT Client

# Assign event callbacks
mqttclient.on_connect = on_connect
mqttclient.on_message = on_message

# Connect to MQTT Client
mqttclient.username_pw_set(Acces_token)
mqttclient.connect(Broker, 1883,1)

# Loops MQTT Client
mqttclient.loop_forever()