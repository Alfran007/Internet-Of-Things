/*
DIGITAL SENSOR CHECK FOR POWER STATUS

Connect the + on the sensor board to +5V on the Arduino
Connect the - on the sensor board to GND on the Arduino

Connect the P on the board to Digital Pin 3 on the Arduino 
*/

/* 
Define the Pins on which the sensors are attached
Power Sensor P is attached to Digital Pin 3 on Arduino
*/
int powerPin= 3; 	

/* Global Varibales to store the power status value */
boolean powerStatus;

/*
This function is called only once in lifetime
Will be called again when the RED RESET button is pressed on the Arduino
*/
void setup()
{
      // Setup the connection speed to Serial Monitor and Arduino Board
      // 9600 bits of data per second
      Serial.begin(9600);

      // Pin 3 defined for taking INPUT
      pinMode(powerPin, INPUT);

      // Print the output on the Serial Monitor
      Serial.println("DIGITAL SENSOR CHECK FOR POWER STATUS");
}

/*
This function is called in a loop again and again by Arduino infinitely 
untill the RED RESET button is not pressed
*/
void loop()
{
        // Call the function to get the power status and store in our global variable
        powerStatus = getPowerStatus(powerPin);
        
        // Print the output on the Serial Monitor
        Serial.println("");
        Serial.println("******************************");
        
        
        Serial.print("Power Status =  ");
        
        if(powerStatus == true)
                Serial.println("ON");

        if(powerStatus == false)
                Serial.println("OFF");
        
        Serial.println("******************************");
        Serial.println("");
        
        // Delay of 1 sec to take the sensor value again
        delay(1000);
} 

/*********** Digital Read function*********/
boolean getPowerStatus(int powerPin)
{  	
         return (boolean)digitalRead(powerPin);
}
