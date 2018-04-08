
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


    GND               7
    o     o     o     o
    GND   IO2   IO0   RX
               
                  
                   
    TX    CH    RST   VCC
    o     o     o     o
    8     3.3         3.3

    ARDUINO      ESP8266
    +3.3V           VCC
    +3.3V           CH_PD
    RXD             PIN 7
    TXD             PIN 8
    GND             GND


  
*/

#include <SoftwareSerial.h>

/* Include the library for using the DHT Sensor */
#include <dht11.h>


//ESP8266 TX is connected to Arduino Digital Pin 8
#define ESP8266_TX_PIN 8
 
//ESP8266 RX is connected to Arduino Digital Pin 7
#define ESP8266_RX_PIN 7

 
                            // Rx        ,        Tx
SoftwareSerial WiFi_Serial(ESP8266_TX_PIN,ESP8266_RX_PIN);

#define WIFI_NAME "Tanmay"
#define WIFI_PASSWORD "123456789"
#define mobile "8657456908"
#define port 80
#define server "api.thingspeak.com"
#define apiKey "6T6HZQHX5DHVUKT5"

String uri="/update?api_key="+String(apiKey);

boolean No_IP=false;

String data="";
String request_header_1="";
String request_header_2=""; 

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

/* Global Variable to access the library functions */
dht11 DHT11;


void setup()
{
      Serial.begin(9600);    
      Serial.println("Sending Sensor Data to ThingsSpeak using AT Command"); 

      pinMode(tempPin, INPUT);
      pinMode(lightPin, INPUT);
      pinMode(humidityPin, INPUT);

      // Pin 2 defined for taking INPUT
      pinMode(reedPin, INPUT);

      // Pin 3 defined for taking INPUT
      pinMode(powerPin, INPUT);

      // Pin 4 defined for taking INPUT
      pinMode(pirPin, INPUT);

      WiFi_Serial.begin(9600);
      wifi_init();
      Serial.println("System Ready and connected to WiFi network");
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
  
        postToThingsSpeak();
}

void postToThingsSpeak()
{
      Serial.println("Connecting to ThingsSpeak Cloud");
      response="";
      
      prepareData();

      Serial.println("AT+CIPSTART=\"TCP\",\"" + String(server)+"\","+ String(port));    
      WiFi_Serial.println("AT+CIPSTART=\"TCP\",\"" + String(server)+"\","+ String(port));
      delay(5000);
      
      if(WiFi_Serial.available())
      {
          response="";
          
          // read the data into a variable as long as the
          while(WiFi_Serial.available())  
            response+= (char)WiFi_Serial.read();
          
          Serial.println(response);
          if(WiFi_Serial.find("CONNECT"))
            Serial.print("AT+CIPSEND=");
          

          WiFi_Serial.print("AT+CIPSEND=");
          WiFi_Serial.println(request_header_1.length() + request_header_2.length() + data.length());
          delay(1000);
          
          if(WiFi_Serial.available());
            response="";

          // read the data into a variable as long as the 
          while(WiFi_Serial.available()) 
            response+= (char)WiFi_Serial.read();
           
          Serial.println(response);

          Serial.println(data);  
          if(WiFi_Serial.find(">"));
          {
            WiFi_Serial.print(request_header_1);
            WiFi_Serial.print(request_header_2);
            WiFi_Serial.print(data);
            delay(5000);
          }
          
          if(WiFi_Serial.available())
          {
            delay(100);
            String response="";
            
            // read the data into a variable as long as the 
            while(WiFi_Serial.available()) 
              response+= (char)WiFi_Serial.read();

            Serial.println(response);
            if(WiFi_Serial.find("200"));
            {
              Serial.println("RESPONSE: 200");
              delay(1000);
            }
          }
      }
      else
      {
          Serial.println("Error in Posting");
          delay(1000);
      }     
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

      request_header_1= "POST " + uri + " HTTP/1.1\r\n"+ "Host: " + String(server) + ":" + String(port) + "\r\n" 
      + "Accept: *" + "/" + "*\r\n" + "Content-Length: " + data.length() + "\r\n" ;
      request_header_2= "Content-Type: application/json\r\n\r\n" ;
}


void wifi_init()
{
      connect_wifi("AT",100);
      connect_wifi("AT+CWMODE=3",100);
      connect_wifi("AT+CWQAP",100);  
      connect_wifi("AT+RST",5000);
      check4IP(5000);
      if(!No_IP)
        connect_wifi("AT+CWJAP=\""+String(WIFI_NAME)+"\",\""+String(WIFI_PASSWORD)+"\"",7000);        
}

void connect_wifi(String cmd, int t)
{
  int temp=0,i=0;
  while(1)
  {
    Serial.println(cmd);
    WiFi_Serial.println(cmd); 
    while(WiFi_Serial.available())
    {
      if(WiFi_Serial.find("OK"))
      i=8;
    }
    delay(t);
    if(i>5)
    break;
    i++;
  }
  if(i==8)
  Serial.println("OK");
  else
  Serial.println("Error");
}



void check4IP(int t1)
{
  int t2=millis();
  while(t2+t1>millis())
  {
    while(WiFi_Serial.available()>0)
    {
      if(WiFi_Serial.find("WIFI GOT IP"))
      {
        No_IP=true;
      }
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



