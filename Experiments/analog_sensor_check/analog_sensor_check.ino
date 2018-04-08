/*
ANALOG SENSOR CHECK FOR TEMPERATURE

Connect the + on the sensor board to +5V on the Arduino
Connect the - on the sensor board to GND on the Arduino

Connect the T on the sensor board to A2 on the Arduino
*/

/* 
Define the Pins on which the sensors are attached
Temperature Sensor T is attached to A2 on Arduino
*/
int tempPin= A2; 	

/* Global Varibales to store the temperature value */
float temp;

/*
This function is called only once in lifetime
Will be called again when the RED RESET button is pressed on the Arduino
*/
void setup()
{
      // Setup the connection speed to Serial Monitor and Arduino Board
      // 9600 bits of data per second
      Serial.begin(9600);

      pinMode(tempPin, INPUT);
    
      // Print the output on the Serial Monitor
      Serial.println("ANALOG SENSOR CHECK FOR TEMPERATURE");
}

/*
This function is called in a loop again and again by Arduino infinitely 
untill the RED RESET button is not pressed
*/
void loop()
{
        // Call the function to get the temperature and store in our global variable
        temp = getTemperature (tempPin);
        
        // Print the output on the Serial Monitor
        Serial.println("");
        Serial.println("******************************");
        
        
        Serial.print("Temperature of Room =  ");
        Serial.println(temp);
        
        
        Serial.println("******************************");
        Serial.println("");
        
        // Delay of 5 sec to take the sensor value again
        delay(5000);
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

