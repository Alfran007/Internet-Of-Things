# -*- coding: utf-8 -*-
"""
Created on Tue Apr 03 12:54:08 2018

@author: Kunal
"""

import pandas as pd
import matplotlib.pyplot as plt

# Number to search data for (default="9928492120")
user_ph_number = raw_input("Enter Phone Number: ")

# Number of Records to Display
no_of_records = raw_input("Enter Number of Records: ")

# Name of the CSV file exported from Thingspeak Server
csv_file = "feeds.csv"

# Names of Columns based on their types
column_names = ['Created_at', 'Entry_ID', 'Temperature', 'Light', 'Humidity',
                'Reed', 'Power', 'Motion','Unique_ID','Mobile_No']

# Read data from CSV file
data = pd.read_csv(csv_file,header=0,names=column_names,
                   dtype={'Unique_ID':str,'Mobile_No':str})   # Changing DataType to String

############## Visualization Section ##############

# Cleaning and Fetching of Data
def visuals_data(number, records):
    
    if number:
        new_data = data[data.columns][data['Mobile_No'] == number]
        if new_data.empty:
            return "User Not Found"
        else:
            new_data = new_data.sort_values(['Created_at'],ascending=False)

    else:
        new_data = data.sort_values(['Created_at'],ascending=False)
        
    if records:
        print "\nNumber of Records Displayed is",records
        new_data = new_data.head(int(records))
        return new_data
    else:
        print "\nNumber of Records Displayed is",records
        return new_data   

# Visualization of the Data
def visualization(sensor_data):
    
    if type(sensor_data) != str:
        # Getting the value for timestamp from the DataFrame created in visuals_data function
        timestamp = sensor_data['Created_at']
        timestamp = [str(i)[11:19] for i in timestamp]   # Beautifing Timestamp 
        
        # Loop to Plot Graph for all the Sensors Data, excluding the TimeStamp and Entry_ID values
        for name in sensor_data[column_names[2:]]:
            if name in ['Unique_ID', 'Mobile_No']:
                i = [val for val in sensor_data[name]]
            else:                
                i = [float(val) for val in sensor_data[name]]
            
            # Plotting the Graphs for Vairous Sensor's Data
            plt.plot(timestamp,i,'-o',color='red')   # Plots the line on the Graph
            plt.xlabel("Time")  # Label for x-axis
            plt.ylabel(name)    # Label for y-axis
            plt.title(name+" Graph")  # Title of Graph\
            
            # Changing x-label from horizontal to vertical to increase visiblity
            plt.xticks(timestamp,rotation='vertical')
            plt.margins(0.1)
            
            plt.grid(True,axis='y')   # Adding Grid to Graph
            plt.show()                # Shows the Graph
        
        return "Task Completed Successfully"
    else:
        return "User Not Found"


############## Function Calling Section ##############

print visualization(visuals_data(user_ph_number, no_of_records))
