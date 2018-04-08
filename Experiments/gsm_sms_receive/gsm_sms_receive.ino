#include <SoftwareSerial.h>

//SIM800 GND is connected to Arduino GND
//SIM800 RX is connected to Arduino D7
//SIM800 TX is connected to Arduino D8

//SIM800 TX is connected to Arduino D8
#define SIM800_TX_PIN 8
 
//SIM800 RX is connected to Arduino D7
#define SIM800_RX_PIN 7

//Create software serial object to communicate with SIM800
//                                  Rx   ,       Tx
SoftwareSerial GSM_Serial(SIM800_TX_PIN,SIM800_RX_PIN); 

int LED=A3;

String response1 = "on";

void setup()
{
  Serial.begin(9600);
  GSM_Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
}



// MAIN LOOP

void loop()
{
  Serial.println("Forsk Technologies");
  gsminit();
  GSM_Serial.println("AT+CMGD=1,4"); //delete all SMS messages whose status is "received read" from the message storage area.
  Serial.println("waiting for SMS");
  Serial.println("ALARM OFF");
  while(true)
  {
    gsm_sms_read();
  }
}

// GSM INITILALIZATION

void gsminit()
{
  GSM_Serial.println("AT");  // Test the response from the modem 
  delay(2000);
  GSM_Serial.println("ATE0");  // Echo Off 
  delay(2000);
  GSM_Serial.println("AT+CMGF=1"); // Sets the mode as Text, 0 for PDU Mode
  delay(2000);
  GSM_Serial.println("AT+CNMI=1,2,0,0,0");  // sets the indicators for receiving an SMS message
  delay(1000);
}


// SMS READ FROM GSM

void gsm_sms_read()
{
  if(GSM_Serial.available())
  {
    String gsm_input="";
    // read the data into a variable as long as the buffer is not empty
    while(GSM_Serial.available()) 
    {
      gsm_input+= (char)GSM_Serial.read();
    }
    Serial.print(gsm_input);
    gsm_input.toLowerCase();
    
    // Check if the message contains the text stored in the response1 variable
    if(find_string(gsm_input,response1)) 
    {
      Serial.println("SMS received");
      Serial.println("LED ON ");
      delay(1000);
      digitalWrite(LED_BUILTIN,HIGH);
      delay(10000);
 
      Serial.println("waiting for SMS");
      Serial.println("LED OFF ");
 
      digitalWrite(LED_BUILTIN,LOW);
      GSM_Serial.println("AT+CMGD=1,4"); //delete all SMS messages whose status is "received read" from the message storage area.
      delay(3000);           
    }
  }
}


// Function to locate a given search string in a given base string

boolean find_string(String base, String search)
{
  // find the length of the base string
  int len = search.length(); 
  
  // Iterate from the beginning of the base string till the end minus length of the substring
  for(int m = 0; m<((base.length()-len)+1);m++)
  {
    // Check if the extracted Substring Matches the Search String
    if(base.substring(m,(m+len))==search) 
    {
      // if it matches exit the function with a true value
      return true; 
    }
  }
  
  // if the above loop did not find any matches, control would come here and return a false value
  return false; 
}

// Function to locate a given search character in a given base string and return its position

boolean find_char_loc(String base, char search)
{
  // Iterate from the beginning of the base string till the end minus length of the substring
  for(int m = 0; m < base.length();m++)
  {
    // Check if the character Matches the Search character
    if(base[m]==search) 
    {
      // if it matches exit the function with the current location value
      return m; 
    }
  }
  // if the above loop did not find any matches, control would come here and return a zero value
  return 0; 
}




