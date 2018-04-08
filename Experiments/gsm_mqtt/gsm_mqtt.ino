/*
this is the code of sending dht11 sensor data to mqtt server(dioty) using arduino 
and the gsm without using AT command and it is tested.

               Arduino                                Gsm
               D8                     |                Rx
               D7                     |                Tx
               Gnd                    |                Gnd

 URL to Check the Data on Thingsboard Cloud 
https://demo.thingsboard.io/dashboards/3833b620-a98f-11e7-a2c4-c7f326cba909?publicId=13ff1e90-acee-11e7-aa09-c7f326cba909

               
*/
// you can check the list of supportable modem with this library in examples 

#define TINY_GSM_MODEM_SIM800                                              // Select your modem:                                    
#include <TinyGsmClient.h>                                                 // Library for the interfacing of arduino with the gsm without using AT command 
#include <PubSubClient.h>                                                  // Arduino Mqtt library
#include <SoftwareSerial.h>


#define TOKEN "kw7lALkNnvcpri3OStkG" // DHT22 Demo Device  and it ID is 382b9fd0-a98f-11e7-a2c4-c7f326cba909
#define server "demo.thingsboard.io"


// Your DIoTY settings for mqtt online cloud broker
#define dioty_id    "sylvesterferns@gmail.com"                            // email address used to register with DIoTY
#define dioty_password  "1b7b49ad"                                          // your DIoTY password received at your email
#define clientNr        "01"                                                // used for client id (increment when deploying
                                    

// Some project settings reqd for DIoTY Broker

#define slash     "/"                                                      // all topics are prefixed with slash and your dioty_id
#define topicConnect    "/SIM800"                               // topic we are publishing to

//float humidity, temp_t;                                                    // variable for holding the data of temp and humidity

// DIoTY requires topic names to start with your userId, so stick it in the front
#define concat(first, second) first second                                 // Defining the concatenation of the string 
const char* connectTopic = concat(concat(slash, dioty_id), topicConnect);  // concatenation the slash and dioty_id and topicConnect

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
PubSubClient mqtt(client);

const char* broker = "mqtt.dioty.co";

// failing to use unique client id's will cause you connection to drop, 
// so make it unique's unique by using your userId and a sequence number
const char* client_id = concat(clientNr, dioty_id);
long lastMsg = 0;
char msg[50];
int value = 0;


void setup()
{
  // Set console baud rate
  Serial.begin(115200);
  delay(10);
  serialSIM800.begin(9600);                                                  // Set GSM module baud rate
  setup_gsm();
  // MQTT Broker setup
  mqtt.setServer(broker, 1883);
}

void setup_gsm() 
{
    delay(10);
   // Restart takes quite some time
   // To skip it, call init() instead of restart()
   Serial.println("Initializing modem...");
   modem.restart();                                                       // Restart the Connection with the gsm module 
   String modemInfo = modem.getModemInfo();                               // Return the information of the module 
   Serial.print("Modem: ");
   Serial.println(modemInfo);

   Serial.print("Waiting for network...");
   if (!modem.waitForNetwork())                                          // Checking the network of your sim card
   {
     Serial.println(" fail");
     while (true);
   }
   Serial.println(" OK");
   Serial.print("Connecting to ");
   Serial.print(apn);
   if (!modem.gprsConnect(apn, dioty_id, dioty_password))
    {
     Serial.println(" fail");
     while (true);
    }
   Serial.println(" connected");
}


void reconnectToDoity() 
{
  
   Serial.print("Connecting to ");
  Serial.print(broker);
  // Loop until we're reconnected
  while (!mqtt.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    ++value;  //connection attempt
    if (mqtt.connect(client_id, dioty_id, dioty_password))                       // checking the connection with the Dioty id and password
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      snprintf (msg, 50, "Successful connected to publish topic #%ld", value);
      Serial.print("Publish message: ");
      Serial.println(msg);
      mqtt.publish(connectTopic, msg, true);// printing the message at the gievn topic
      
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void loop() {

  if (!mqtt.connected()) {
      reconnectToDoity();
  } 
 mqtt.loop();
    // Reconnect every 10 seconds
//small code snippet to increment value for every publish

 delay(1000);
  publishToDoity();

}

void publishToDoity()
{
  Serial.println("Collecting temperature data.");

  float h = 35.5;
  
  float t = 26.5;

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"temperature\":"; 
  payload += String(t); 
  payload += ",";
  payload += "\"humidity\":"; 
  payload += String(h);
  payload += "}";
        
  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  Serial.print("Publish message: ");
  Serial.println(attributes);
  mqtt.publish(connectTopic, attributes, true);
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
  mqtt.publish( "v1/devices/me/telemetry", attributes );
  Serial.println( attributes );
}

void reconnectToThingsBoard() 
{
  // Loop until we're reconnected
  while (!mqtt.connected()) 
  {
    Serial.print("Connecting to Thingsboard node ...");
    // Attempt to connect (clientId, username, password)
    if ( mqtt.connect("Arduino Uno Device", TOKEN, NULL) ) 
    {
      Serial.println( "[DONE]" );
    } 
    else 
    {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( mqtt.state() );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}



