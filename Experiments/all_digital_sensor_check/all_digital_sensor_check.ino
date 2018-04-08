/*
DIGITAL SENSOR CHECK FOR 3 DIGITAL SENSORS

Connect the + on the sensor board to +5V on the Arduino
Connect the - on the sensor board to GND on the Arduino

Connect the R on the board to Digital Pin 2 on the Arduino 
Connect the P on the board to Digital Pin 3 on the Arduino 
Connect the X on the board to Digital Pin 4 on the Arduino 
*/

/* 
Define the Pins on which the sensors are attached
Reed Sensor R is attached to Digital Pin 2 on Arduino
Power Sensor P is attached to Digital Pin 3 on Arduino
PIR Sensor X is attached to Digital Pin 4 on Arduino
*/
int reedPin= 2; 	
int powerPin= 3;   
int pirPin= 4;   

/* Global Varibales to store the reed status value */
boolean reedStatus;

/* Global Varibales to store the power status value */
boolean powerStatus;

/* Global Varibales to store the pir status value */
boolean pirStatus;

/*
This function is called only once in lifetime
Will be called again when the RED RESET button is pressed on the Arduino
*/
void setup()
{
      // Setup the connection speed to Serial Monitor and Arduino Board
      // 9600 bits of data per second
      Serial.begin(9600);
    
      // Pin 2 defined for taking INPUT
      pinMode(reedPin, INPUT);

      // Pin 3 defined for taking INPUT
      pinMode(powerPin, INPUT);

      // Pin 4 defined for taking INPUT
      pinMode(pirPin, INPUT);

      // Print the output on the Serial Monitor
      Serial.println("DIGITAL SENSOR CHECK FOR 3 DIGITAL SENSORS");
}

/*
This function is called in a loop again and again by Arduino infinitely 
untill the RED RESET button is not pressed
*/
void loop()
{
        // Call the function to get the power status and store in our global variable
        reedStatus = getReedStatus(reedPin);
        powerStatus = getPowerStatus(powerPin);
        pirStatus = getPirStatus(pirPin);
               
        // Print the output on the Serial Monitor
        Serial.println("");
        Serial.println("******************************");
        
        
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
        
        // Delay of 1 sec to take the sensor value again
        delay(1000);
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


