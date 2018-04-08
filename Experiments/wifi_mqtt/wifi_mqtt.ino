/*
 URL to Check the Data on Thingsboard Cloud 
https://demo.thingsboard.io/dashboards/3833b620-a98f-11e7-a2c4-c7f326cba909?publicId=13ff1e90-acee-11e7-aa09-c7f326cba909


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

#include <WiFiEspClient.h>
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include <PubSubClient.h>
#include "SoftwareSerial.h"

#define WIFI_AP "anto"
#define WIFI_PASSWORD "21122112"
#define TOKEN "kw7lALkNnvcpri3OStkG" // DHT22 Demo Device  and it ID is 382b9fd0-a98f-11e7-a2c4-c7f326cba909
#define server "demo.thingsboard.io"

//ESP8266 TX is connected to Arduino Digital Pin 8
#define ESP8266_TX_PIN 8
 
//ESP8266 RX is connected to Arduino Digital Pin 7
#define ESP8266_RX_PIN 7

//char thingsboardServer[] = "demo.thingsboard.io";


// Initialize the Ethernet client object
WiFiEspClient espClient;
PubSubClient client(espClient);

                     // Rx        ,        Tx
SoftwareSerial soft(ESP8266_TX_PIN,ESP8266_RX_PIN);

int status = WL_IDLE_STATUS;


unsigned long lastSend;

void setup() 
{
    Serial.begin(9600);
    InitWiFi();
    client.setServer( server, 1883 );
    lastSend = 0;
}

void loop() 
{
  status = WiFi.status();
  if ( status != WL_CONNECTED) 
  {
    while ( status != WL_CONNECTED) 
    {
      Serial.print("Attempting to connect to WPA SSID: ");
      Serial.println(WIFI_AP);
      // Connect to WPA/WPA2 network
      status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      delay(500);
    }
    Serial.println("Connected to AP");
  }

  if ( !client.connected() ) 
  {
    reconnect();
  }

  if ( millis() - lastSend > 1000 ) 
  { 
    // Update and send only after 1 seconds
    // Call the function to publish to the Server
    publishToThingsBoard();
    lastSend = millis();
  }
  client.loop();
}


void publishToThingsBoard()
{
  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"temperature\":"; 
  payload += String(24.5); 
  payload += ",";
/*
  payload += "\"mobile\":"; 
  payload += String("982849210"); 
  payload += ",";

  payload += "\"light\":"; 
  payload += String(245); 
  payload += ",";
  
  payload += "\"door\":"; 
  payload += String(1); 
  payload += ",";

  payload += "\"power\":"; 
  payload += String(0); 
  payload += ",";

  payload += "\"person\":"; 
  payload += String(1); 
  payload += ",";

  payload += "\"unique\":"; 
  payload += String("9928492120"); 
  payload += ",";
*/ 
  payload += "\"humidity\":"; 
  payload += String("46");
  payload += "}";

  // Send payload
  char attributes[200];
  payload.toCharArray( attributes, 200 );
//  client.publish( "v1/devices/me/telemetry", attributes );
  client.publish( "v1/devices/me/attributes", attributes );
  Serial.println( attributes );
}

void InitWiFi()
{
  // initialize serial for ESP module
  soft.begin(9600);
  // initialize ESP module
  WiFi.init(&soft);
  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) 
  {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) 
  {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_AP);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    delay(500);
  }
  Serial.println("Connected to AP");
}

void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Connecting to Thingsboard node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("Arduino Uno Device", TOKEN, NULL) ) 
    {
      Serial.println( "[DONE]" );
    } 
    else 
    {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}



