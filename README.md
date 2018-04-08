# Internet-Of-Things

1) Used sensor to monitor temperature(LM35D), light intensity(LDR), humidity(DHT11), magnetic field(REED), optocoupler(PC817), power, 
location(GPS) and human presence(PIR).
   
2) Connected Arduino with the these sensors on the corresponding pins via sensor board and connected it with the computer.

3) Using Arduino Software a code is generated in setup and loop which gives instruction to the arduino board and read and write on the 
   corresponding pin.
   
4) Rest API is used and an online website for storing the data(thingspeak.com here) and a channel is created having the corresponding 
   fields same as the data we get from Arduino via sensors.
   
5) This data is stored and sent to cloud(on our channel) using wifi(ESP8266){which gets connected with our hotspot} or we can use 
   sim(SIM800), API key of site is used for authentication and finally the data gets stored in JSON format on our channel{In Public and 
   Private View one can see the charts created for the data}.
   
6) Then this JSON data gets exported from cloud and is hosted on a server(MongoDB){I tried both local and remote python files are 
   included in experiments directory} on mLab webite(One have to enable API key and add it in your python code). Finally we have a hosted   
   cloud server which provides us usefull information.
   
7) In pi directory some codes are given for same operation in RaspberryPi. 
   

   
 ### We use this type information in various filds like for false claims in transport for the damaged products.
