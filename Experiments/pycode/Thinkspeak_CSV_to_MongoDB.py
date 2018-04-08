# -*- coding: utf-8 -*-
"""
Created on Mon Feb 26 17:09:38 2018

@author: Kunal
"""

import json
import urllib2
import pandas as pd
from pymongo import MongoClient
import matplotlib.pyplot as plt

# Creating Object for MongoDB Instance
client = MongoClient('localhost', 27017)

# Initializing Database (ThingSpeak) and creating its object
my_db = client.ThingSpeak

# Name of the CSV file exported from Thingspeak Server
csv_file = "feeds.csv"

# Credentials for SMS service
sms_ph_number = "8657456908"    # Number to send SMS alert to

# range for Temperature 
temp_uppr_limit = 40
temp_lowr_limit = 5


############## Data To MongoDB Section ##############

def data_to_mongo(filename):
    try:
        my_db.drop_collection("Data")          # Drop the existing collection
        data = pd.read_csv(filename)           # Read data from CSV file
        data = data.to_json(orient='records')  # Convert data from CSV format to JSON format
        json_data = json.loads(data)           # Parse data to JSON
        
        # Insert JSON data to collection (Data) in Thingspeak Database
        my_db.Data.insert_many(json_data)
        
        return "Data Added to MongoDB"
    
    except Exception as e:
        return "ERROR :",str(e)  # Returns Error if any


############## SMS Section ##############

def sms(number,temp): 

    # Alert Message to be sent
    message = "SMS ALERT Room Temperature is {0}C".format(temp)
    message = "+".join(message.split(' '))

    # Adding number and message to SMS url.(P.S. rest all the attributes remains same as per service provider)
    url ='http://sms.dataoxytech.com/index.php/smsapi/httpapi/?uname=sylvester007&password=forskmnit&sender=FORSKT&receiver={0}&route=TA&msgtype=1&sms={1}'.format(number,message)

    try:
        send_sms = urllib2.urlopen(url)  # Hitting the SMS url
    except Exception as e:
        return "Error:",str(e)
    return send_sms.read() # Returns SMS Response


############## Alert Section ##############

def temp_alert(number):
    
    # Searching for the existing user in MongoDB
    user = my_db.Data.find_one({"field8" : int(number)}, {"id" : 0})
    
    # Checking is the user Exists
    if user:
        temp = float(user["field1"])    # Getting Temperature Value for that user
        
        # checking if the Temperature is in Safe limits or not
        if not(temp_lowr_limit < temp < temp_uppr_limit):
            return sms(number,temp) # Calls SMS function to send alert to user
        else:
            return "Temperature Under Control"
    else:
        return "User Not Found"


############## Visualization Section ##############

def visuals_data(number):
    
    # Searching for the existing user in MongoDB
    user = my_db.Data.find({"field8" : int(number)}, {"id" : 0})
    
    if user:
        # Creating Blank lists to store data
        t,l,h,r,p,m,ts=[],[],[],[],[],[],[]
        
        # Adding each sensor data to their respective list
        for obj in user:
            ts.append(obj["created_at"])
            t.append(obj["field1"])
            l.append(obj["field2"])
            h.append(obj["field3"])
            r.append(obj["field4"])
            p.append(obj["field5"])
            m.append(obj["field6"])
        return (t,l,h,r,p,m,ts) # Returns Tuple of all the Lists
    else:
        return "User Not Found"  
    

def visualization(sensor_data):
    
    if type(sensor_data) == tuple:
        # Getting the value for timestamp from the Tuple created in visuals_data function i.e. ts
        timestamp = sensor_data[-1]
        
        # Loop to Plot Graph for all the Sensors Data, excluding the ts value
        for i in sensor_data[:-1]:
            plt.plot(timestamp,i)     # Plots the line on the Graph
            plt.scatter(timestamp,i)  # Plots the Dots on the Graph
            plt.xlabel("Time")        # Label for x-axis
            plt.show()                # Shows the Graph
        return "Task Completed Successfully"
    else:
        return "User Not Found"


############## Function Calling Section ##############

print data_to_mongo(csv_file)

print temp_alert(sms_ph_number)

print visualization(visuals_data(sms_ph_number))
