# Internet-Of-Things

*  1 )  Used sensors to monitor temperature(**LM35D**), light intensity(**LDR**), humidity(**DHT11**), magnetic field(**REED**),  
   optocoupler(**PC817**), power(**Switch**), location(**GPS**) and human presence(**PIR**).
   
   - These sensors looks like below:
   
   ![devices](https://user-images.githubusercontent.com/19959305/38469969-4c0292d4-3b7a-11e8-9463-67ebd4c2493a.png)
   
*  2. Connected Arduino with the these sensors on the corresponding pins via sensor board and connected it with the computer.
   __Arduino__ and Sensor board looks like below:
   
   ![arduino](https://user-images.githubusercontent.com/19959305/38470477-3f7d2036-3b81-11e8-8abb-fe0d8eb64c29.png)
   
   ![sensor](https://user-images.githubusercontent.com/19959305/38470016-dc87c504-3b7a-11e8-8118-782daebe70e7.png)

*  3. Using Arduino Software a code is generated in setup and loop which gives instruction to the arduino board and read and write on the corresponding pin.

   - The overall setup would be like shown:
   
   ![circuit](https://user-images.githubusercontent.com/19959305/38470170-8f8f1502-3b7c-11e8-93f0-779d428f04cc.jpeg)
   
*  4. Rest API is used and an online website for storing the data([here](https://thingspeak.com/)) and a channel is 
   created having the corresponding fields same as the data we get from Arduino via sensors.
   
*  5. This data is stored and sent to cloud(on our channel) using wifi(ESP8266){which gets connected with our hotspot} or we can use 
   sim(SIM800), API key of site is used for authentication and finally the data gets stored in JSON format on our channel{In Public 
   and Private View one can see the charts created for the data}.
   
   - Data analysis looks like below:
   ![think](https://user-images.githubusercontent.com/19959305/38470067-69b499f2-3b7b-11e8-996f-6720a904193c.png)
   
*  6. Then this JSON data gets exported from cloud and is hosted on a server(MongoDB){I tried both local and remote python files are 
   included in experiments directory} on mLab webite(One have to enable API key and add it in your python code). Finally we have a 
   hosted cloud server which provides us usefull information.
   
   - The running mongo instance on cloud(mlab) would look like this:
   ![mlab](https://user-images.githubusercontent.com/19959305/38469917-bff61dec-3b79-11e8-96d7-510a723d747c.png)
   
   - JSON data that would be visible in mongo instance created is shown:
   ![data](https://user-images.githubusercontent.com/19959305/38470120-1785b142-3b7c-11e8-9648-ec9a1365c9c2.png)

   
*  7. In pi directory some codes are given for same operation in __RaspberryPi__. 
   

   
 ### We use this type of project in various filds like for false claims in transport for the damaged products.
