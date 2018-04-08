/*
 URL to Check the Data on Cloud using sylvesterf as Author and Channel ID 223662
https://thingspeak.com/channels/223662
*/

#define TINY_GSM_MODEM_SIM800 
#include <TinyGsmClient.h>                                                                                                                                                          
#include <SoftwareSerial.h>

#define server "api.thingspeak.com"
#define apiKey "J0WJOEXGJNF0QNC8"
#define mobile "9928492120"

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

void setup()
{
    // Set console baud rate
    Serial.begin(115200);
    delay(10);
    
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
       // Checking connection with the client
       if ( !client.connected() )                   
       {
           // If the client is not connected then reconnect
           reconnect();                              
       }
       Serial.println(" connected");
       
       // Function is creating JSON and posting the data to the server
       createJSONandPostData();      

       // After successfull posting the data on server stoping the client communication.
       // if you don't do this then the connection remain open hence it will send the data once or twice.
                         
       client.stop();                          

       // There needs to be atleast 15 sec delay in sending the data                                         
       delay(20000);                             
}

void createJSONandPostData()
{
          Serial.println("Creating JSON");
      
          // Create the JSON for the data to be uploaded on the Cloud
    
          String data;
          
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
    
    
         Serial.println(data);                                             //printing the data on serial monitor
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
      if ( client.connect(server, 80) )
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
