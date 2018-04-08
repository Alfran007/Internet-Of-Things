# -*- coding: utf-8 -*-
"""
Created on Thu Mar 08 14:12:54 2018

@author: Kunal
"""

# Required Libraries
import json
import urllib2
import requests
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Api Key for MLab Database
Api_Key = "PhjKu8FiCCEQY2K60eX_CwCARrRcLvB6"

# Credintials for MLab Database
my_db = "thingspeak"      # Database Name
my_collection = "Data"    # Collection Name

# Name of the CSV file exported from Thingspeak Server
csv_file = "feeds.csv"

# Base URL for MLab API
base_url = "https://api.mlab.com/api/1"

# Credentials for SMS service
sms_ph_number = "8657456908"    # Number to send SMS alert to

# range for Temperature 
temp_uppr_limit = 40
temp_lowr_limit = 5


############## Data To MLab Section ##############

def data_to_MLab(filename):
    ''' Inserting Data to MLab Database '''
    
    try:
        # Creating MLab URL with Database name ,Collection name and API_Key
        url = base_url+"/databases/"+my_db+"/collections/"+my_collection+"?apiKey="+Api_Key
        
        data = pd.read_csv('feeds.csv')        # Read data from CSV file
        data = data.dropna()                   # Dropping rows with missing data
        data = data.to_json(orient='records')  # Convert data from CSV format to JSON format
        json_data = json.loads(data)           # Parsing JSON data to Dictionary
        
        # Sending Data as JSON to MLab using POST request
        res = requests.post(url,json=json_data)
        
        # Getting the MLab Response
        response = json.loads(res.text)
        
        if response.has_key('n'):
            return  "Data Added Successfully"
        else:
            return response
    
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


############## Alert Section ##############

def temp_alert(number):
    ''' Finding user with Phone number and Sending Alert if required '''
    
    # Searching for the existing user in MLab
    url = base_url+"/databases/"+my_db+"/collections/"+my_collection+"?apiKey="+Api_Key+"&fo=true&q={'field8': "+number+"}"
    
    user = requests.get(url)
    user = json.loads(user.text)

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
    ''' Fetching Data from MLab and getting it ready to be plotted '''
    
    # Searching for the existing user in MLab
    url = base_url+"/databases/"+my_db+"/collections/"+my_collection+"?apiKey="+Api_Key+"&q={'field8': "+number+"}&s={'created_at':-1}&l=20"
    
    user = requests.get(url)
    user = json.loads(user.text)
    
    if user:
        # Creating Blank lists to store data
        t,l,h,r,p,m,uid,mob,ts=[],[],[],[],[],[],[],[],[]
        
        # Adding each sensor data to their respective list
        for obj in user:
            ts.append(obj["created_at"])
            t.append(obj["field1"])
            l.append(obj["field2"])
            h.append(obj["field3"])
            r.append(obj["field4"])
            p.append(obj["field5"])
            m.append(obj["field6"])
            uid.append(obj["field7"])
            mob.append(obj["field8"])
       
        return (t,l,h,r,p,m,uid,mob,ts) # Returns Tuple of all the Lists
    else:
        return "User Not Found"  
    

def visualization(sensor_data):
    ''' Plotting Data received from MLab by using MatPlotLib.pyplot '''
    
    if type(sensor_data) == tuple:
        # Getting the value for timestamp from the Tuple created in visuals_data function i.e. ts
        timestamp = sensor_data[-1]
        timestamp = [str(time)[11:19] for time in timestamp]   # Beautifing TimeStamp
        
        # Creating List of all the data field names
        fields = ['Temperature', 'Light', 'Humidity', 'Reed', 'Power', 'Motion', 'Unique_ID', 'Mobile_no']
        
        # Loop to Plot Graph for all the Sensors Data, excluding the timestamp value
        for value,name in zip(sensor_data[:-1],fields):
            
            # Check to separate Unique_ID and Mobile_no from sensor data
            if name=='Unique_ID' or name=='Mobile_no':
                value = [str(item) for item in value]
            else:                
                value = [float(item) for item in value]
            
            plt.plot(timestamp,value,'-o',color='red')     # Plots the data on the Graph
            plt.xlabel("Time")     # Label for x-axis
            plt.ylabel(name)       # Label for y-axis
            plt.title(name+" Graph")    # Title of the Graph
            
            # Changing display style of timestamp from horizontal to vertical
            plt.xticks(timestamp,rotation='vertical')
            plt.margins(0.1)   # Providing Space between x-labels
                        
            # Check to set y-axis range as 0 and 1 for Digital Sensors
            if max(value)==0.0 or max(value)==1.0:
                plt.yticks(np.arange(min(value), max(value)+1, 1.0))
                
            plt.grid(True,axis='y')   # Adding Grid lines to Graph (only for y-axis)
            plt.show()                # Shows the Graph
        
        return "Task Completed Successfully"
    else:
        return "User Not Found"


############## Function Calling Section ##############

mlab_response = data_to_MLab(csv_file)  # Inserting Data to MLab server

print mlab_response

if mlab_response == "Data Added Successfully":
    #print temp_alert(sms_ph_number)  # Alert User if temperature is higer than threshold value    
    print visualization(visuals_data(sms_ph_number))   # Visualization of the Sensor Data