# -*- coding: utf-8 -*-
"""
Created on Fri Apr 06 14:44:35 2018

@author: Kunal
"""

import pandas as pd


# Names of Columns based on their types
column_names = ['Created_at', 'Entry_ID', 'Temperature', 'Light', 'Humidity',
                'Reed', 'Power', 'Motion','Unique_ID','Mobile_No']

# Read Data from CSV file
Data = pd.read_csv('feeds.csv',header=0,names=column_names,
                   dtype={'Unique_ID':str,'Mobile_No':str})   # Changing DataType to String

Data = Data.dropna()     # Removing rows with missing values (if any)

# Getting Minimum Temperature, and rounding it to two decimal values
Min_temp = round(Data['Temperature'].min(),2)

# Getting Maximum Temperature, and rounding it to two decimal values
Max_temp = round(Data['Temperature'].max(),2)

# Getting Average Temperature, and rounding it to two decimal values
Avg_temp = round(Data['Temperature'].mean(),2)

# Getting Middle Value of Temperature, and rounding it to two decimal values
Middle_temp_value = round(Data['Temperature'].median(),2)


# Getting Most Frequent Temperature, and rounding it to two decimal values
Most_frequent_temp = round(Data['Temperature'].mode()[0],2)