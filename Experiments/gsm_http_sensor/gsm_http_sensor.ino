/*
 
Connect the + on the sensor board to +5V on the Arduino
Connect the - on the sensor board to GND on the Arduino

Connect the T on the sensor board to A2 on the Arduino for Temperature
Connect the L on the sensor board to A3 on the Arduino for Light
Connect the H on the sendor board to A4 on the Arduino for Humidity

Connect the R on the board to 2 on the Arduino 
Connect the P on the board to 3 on the Arduino 
Connect the X on the board to 4 on the Arduino 

 URL to Check the Data on Cloud using sylvesterf as Author and Channel ID 223662
https://thingspeak.com/channels/223662
*/

#define TINY_GSM_MODEM_SIM800 
#include <TinyGsmClient.h>                                                                                                                                                          
#include <SoftwareSerial.h>
/* Include the library for using the DHT Sensor */
#include <dht11.h>

/* Global Variable to access the library functions */
dht11 DHT11;


// Your GPRS Internet APN  based on the SIM Card
// Jio is NOT compatible
const char apn[]  = "imis/internet";

//APN for airtel:- airtelgprs.com 
// APN for idea:- imis/internet
// APN for vodafone:-   www
// APN for reliance:- rcomnet
// APN for bsnl:- bsnlnet
// APN for Aircel:- aircelgprs.pr

//SIM800 GND is connected to Arduino GND
//SIM800 RX is connected to Arduino D7
//SIM800 TX is connected to Arduino D8

//SIM800 TX is connected to Arduino D8
#define SIM800_TX_PIN 8
 
//SIM800 RX is connected to Arduino D7
#define SIM800_RX_PIN 7
 
//Create software serial object to communicate with SIM800
//                                  Rx   ,       Tx
SoftwareSerial serialSIM800(SIM800_TX_PIN,SIM800_RX_PIN);

TinyGsm modem(serialSIM800);
TinyGsmClient client(modem);

#define server "api.thingspeak.com"
#define apiKey "J0WJOEXGJNF0QNC8"
#define mobile "9928492120"
#define port 80

String uri="/update?api_key="+String(apiKey);
String data="";
String response="";

/* 
Define the Pins on which the sensors are attached
Temperature Sensor T is attached to A2 on Arduino
Light Sensor L is attached to A3 on Arduino
Humidity Sensor H is attached to A4 on Arduino
*/

int tempPin= A2;   
int lightPin = A3;
int humidityPin = A4;

/* 
Define the Pins on which the sensors are attached
Reed Sensor R is attached to Digital Pin 5 on Arduino
Power Sensor P is attached to Digital Pin 6 on Arduino
PIR Sensor X is attached to Digital Pin 7 on Arduino
*/
int reedPin= 2;   
int powerPin= 3;   
int pirPin= 4;   

/* Global Variable to store the temperature value */
float temp;

/* Global Variable to store the light value */
float light;

/* Global Variable to store the humidity value */
float humidity;

/* Global Variable to store the reed status value */
boolean reedStatus;

/* Global Variable to store the power status value */
boolean powerStatus;

/* Global Variable to store the pir status value */
boolean pirStatus;



void setup()
{
    // Set console baud rate
    Serial.begin(9600);
    delay(10);

    Serial.println("Sending Sensor Data to ThingsSpeak"); 

    pinMode(tempPin, INPUT);
    pinMode(lightPin, INPUT);
    pinMode(humidityPin, INPUT);

    // Pin 2 defined for taking INPUT
    pinMode(reedPin, INPUT);

    // Pin 3 defined for taking INPUT
    pinMode(powerPin, INPUT);

    // Pin 4 defined for taking INPUT
    pinMode(pirPin, INPUT);


    // Set GSM module baud rate
    serialSIM800.begin(9600);                              

    // Initializing the GSM
    setup_gsm();
}


void setup_gsm() 
{
     delay(10);
     // Restart takes quite some time
     // To skip it, call init() instead of restart()
     Serial.println("Initializing modem...");
     
     // Restart the Connection with the gsm module 
     modem.restart();            

     // Return the information of the module                     
     String modemInfo = modem.getModemInfo();          

     Serial.print("Modem: ");
     Serial.println(modemInfo);
     Serial.print("Waiting for network...");

     // Checking the network of your sim card
     if (!modem.waitForNetwork())                     
     {
       Serial.println(" fail");
       while (true);
     }
     Serial.println(" OK");
     Serial.print("Connecting to ");
     Serial.print(apn);
     if (!modem.gprsConnect(apn, "", ""))
      {
       Serial.println(" fail");
       while (true);
      }
     Serial.println(" connected");
}


void loop()
{
        // Call the function to get the status and store in our global variable
        temp = getTemperature (tempPin);
        light = getLight(lightPin);
        humidity = getHumidity(humidityPin);
        
        reedStatus = getReedStatus(reedPin);
        powerStatus = getPowerStatus(powerPin);
        pirStatus = getPirStatus(pirPin);
               
        // Print the output on the Serial Monitor
        Serial.println("");
        Serial.println("******************************");
        
        Serial.print("Temperature of Room =  ");
        Serial.println(temp);

        Serial.print("Light Intensity in Room =  ");
        Serial.println(light);

        Serial.print("Humidity of Room =  ");
        Serial.println(humidity);
 
        Serial.print("Gate Status =  ");     
        if(reedStatus == true)
                Serial.println("OPEN");
        if(reedStatus == false)
                Serial.println("CLOSE");

        Serial.print("Power Status =  ");
        if(powerStatus == true)
                Serial.println("ON");
        if(powerStatus == false)
                Serial.println("OFF");

        Serial.print("Person Status =  ");
        if(pirStatus == true)
                Serial.println("YES");
        if(pirStatus == false)
                Serial.println("NO");
 
        Serial.println("******************************");
        Serial.println("");


       // Checking connection with the client
       if ( !client.connected() )                   
       {
           // If the client is not connected then reconnect
           reconnect();                              
       }
       Serial.println(" connected");
       
       // Function is creating JSON and posting the data to the server
       postToThingsSpeakGSM();      

       // After successfull posting the data on server stoping the client communication.
       // if you don't do this then the connection remain open hence it will send the data once or twice.
                         
       client.stop();                          

       // There needs to be atleast 15 sec delay in sending the data                                         
       delay(20000);                             
}

void prepareData()
{
      data = "";
      data += "{";
      data += "\"field1\":\""+String(temp)+"\",";       // Temperature 
      data += "\"field2\":\""+String(light)+"\",";      // Light
      data += "\"field3\":\""+String(humidity)+"\",";   // Humidity   
      data += "\"field4\":\""+String(reedStatus)+"\","; // Door Status ( OPEN=1 or CLOSE=0)
      data += "\"field5\":\""+String(powerStatus)+"\",";// Power Status ( ON=1 or OFF=0 )
      data += "\"field6\":\""+String(pirStatus)+"\",";  // Person Status ( YES=1 or NO=0 )    
      data += "\"field7\":\""+String(mobile)+"\",";     // Unique ID
      data += "\"field8\":\""+String(mobile)+"\"";      // Mobile number
      data += "}";

}


     
void postToThingsSpeakGSM()
{
       Serial.println("Connecting to ThingsSpeak Cloud");
       response="";
      
       prepareData();
         
         //Making http post request
         client.println("POST /update?api_key="+String(apiKey)+" HTTP/1.1");
         client.println("Host: "+String(server));
         client.println("Accept: */*");
         client.println("Content-Length: " + String(data.length()));
         client.println("Content-Type: application/json");
         client.println();
         client.println(data);   
}

void reconnect() 
{
      // Loop until we're reconnected
      while (!client.connected())
       {
      Serial.print("Connecting to Thingspeak server ...");
      // Attempt to connect (clientId, username, password)
      if ( client.connect(server, port) )
         {
          Serial.println( "Connected" );
         } 
         else 
            {
             Serial.print( "[FAILED] [ rc = " );
             // Serial.print( client.state() );
             Serial.println( " : retrying in 5 seconds]" );
            // Wait 5 seconds before retrying
            delay( 5000 );
            }
      }
}



/*********** Analog Read function*********/
float getTemperature(int tempPin)
{    
        // ananlogRead function converts the input voltage range (0 to 5V), to a digital value between (0 to 1023)
        // This is done by a circuit inside the microcontroller called an analog-to-digital converter or ADC. 
        // Read the value from the Analog Pin and store it to an integer variable
        int sensorValue = analogRead(tempPin);   

        // To scale the numbers between 0.0 and 5.0, 
        // divide 5.0 by 1023.0 and multiply that by sensorValue
        // multiply the result with reference milli volts. (1V = 1000mV)
        float milliVoltsTemp = sensorValue*(5.0/1023.0)*1000;

        // There will be 1°C change for every 10mV of output
        return milliVoltsTemp/10;
}


float getHumidity(int humidityPin)
{
        // Read the sensor value using the dht library read function 
        DHT11.read(humidityPin);

        // After reading the value which is stored in the library variable
        return (float)DHT11.humidity;
}


float getLight(int lightPin)
{
        // ananlogRead function converts the input voltage range (0 to 5V), to a digital value between (0 to 1023)
        // This is done by a circuit inside the microcontroller called an analog-to-digital converter or ADC. 
        // Read the value from the Analog Pin and store it to an integer variable
        int sensorValue=analogRead(lightPin);

        // To scale the numbers between 0.0 and 5.0, 
        // divide 5.0 by 1023.0 and multiply that by sensorValue
        // multiply the result with reference milli volts. (1V = 1000mV)
        float milliVoltsLight =sensorValue*(5.0/1023.0)*1000;

        // There will be 1 Lumen change for every 10mV of output
        return milliVoltsLight/10;
}


/*********** Digital Read function*********/
boolean getReedStatus(int reedPin)
{    
        return (boolean)digitalRead(reedPin);        
}

boolean getPowerStatus(int powerPin)
{    
        return (boolean)digitalRead(powerPin);
}

boolean getPirStatus(int pirPin)
{    
        return (boolean)digitalRead(pirPin);      
}



