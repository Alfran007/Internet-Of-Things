/*

URL to Check the Data on Cloud using sylvesterf as Author and Channel ID 223662 
once you have successfully run the project and able to see the data sent from the 
serial monitor 
 
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

void setup()
{
      Serial.begin(9600);    
      Serial.println("Sending Data to ThingsSpeak using AT Command"); 
  
      WiFi_Serial.begin(9600);
      wifi_init();
      Serial.println("System Ready and connected to WiFi network");
}

void loop()
{
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
      data += "\"field1\":\""+String(22.4)+"\",";       // Temperature 
      data += "\"field2\":\""+String(260)+"\",";        // Light
      data += "\"field3\":\""+String(31)+"\",";         // Humidity   
      data += "\"field4\":\""+String(1)+"\",";          // Door Status ( OPEN=1 or CLOSE=0)
      data += "\"field5\":\""+String(0)+"\",";          // Power Status ( ON=1 or OFF=0 )
      data += "\"field6\":\""+String(0)+"\",";          // Person Status ( YES=1 or NO=0 )    
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


