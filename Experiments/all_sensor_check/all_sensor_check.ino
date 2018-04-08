/*
ALL SENSOR CHECK

Connect the + on the sensor board to +5V on the Arduino
Connect the - on the sensor board to GND on the Arduino

Connect the T on the sensor board to A2 on the Arduino for Temperature
Connect the L on the sensor board to A3 on the Arduino for Light
Connect the H on the sendor board to A4 on the Arduino for Humidity

Connect the R on the board to Digital Pin 2 on the Arduino 
Connect the P on the board to Digital Pin 3 on the Arduino 
Connect the X on the board to Digital Pin 4 on the Arduino 
*/

/* Include the library for using the DHT Sensor */
#include <dht11.h>

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
Reed Sensor R is attached to Digital Pin 2 on Arduino
Power Sensor P is attached to Digital Pin 3 on Arduino
PIR Sensor X is attached to Digital Pin 4 on Arduino
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
      pinMode(lightPin, INPUT);
      pinMode(humidityPin, INPUT);
      
      // Pin 2 defined for taking INPUT
      pinMode(reedPin, INPUT);

      // Pin 3 defined for taking INPUT
      pinMode(powerPin, INPUT);

      // Pin 4 defined for taking INPUT
      pinMode(pirPin, INPUT);

      // Print the output on the Serial Monitor
      Serial.println("ALL SENSOR CHECK");
}

/*
This function is called in a loop again and again by Arduino infinitely 
untill the RED RESET button is not pressed
*/
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


