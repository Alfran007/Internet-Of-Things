/*
ANALOG SENSOR CHECK FOR ALL 3 SENSORS

Connect the + on the sensor board to +5V on the Arduino
Connect the - on the sensor board to GND on the Arduino

Connect the T on the sensor board to A2 on the Arduino for Temperature
Connect the L on the sensor board to A3 on the Arduino for Light
Connect the H on the sendor board to A4 on the Arduino for Humidity

Code Challenge 1
Create a two functions to convert the Value of Temperature in Fahrenheit and Kelvin

Code Challenge 2
Create a new function to read the value of Temperature from the DHT Sensor using Library
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


/* Global Variables to store the temperature value */
float temp;

/* Global Variables to store the light value */
float light;

/* Global Variables to store the humidity value */
float humidity;

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
      
    
      // Print the output on the Serial Monitor
      Serial.println("ANALOG SENSOR CHECK FOR ALL 3 SENSORS");
}

/*
This function is called in a loop again and again by Arduino infinitely 
untill the RED RESET button is not pressed
*/
void loop()
{
        // Call the function to get the Temperature, Light and Humidity
        // and store in our global variables
        temp = getTemperature (tempPin);
        light = getLight(lightPin);
        humidity = getHumidity(humidityPin);
                
        // Print the output on the Serial Monitor
        Serial.println("");
        Serial.println("**********************************");
        
        Serial.print("Temperature of Room =  ");
        Serial.println(temp);

        Serial.print("Light Intensity in Room =  ");
        Serial.println(light);

        Serial.print("Humidity of Room =  ");
        Serial.println(humidity);
 
        Serial.println("**********************************");
        Serial.println("");

        // Delay of 5 sec to take the sensor values again
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
/*
      DHT11 Sensor 
      
      Humidity Range: 20-90% RH
      Humidity Accuracy: ±5% RH
      Temperature Range: 0-50 °C
      Temperature Accuracy: ±2% °C
      Operating Voltage: 3V to 5.5V

*/    
