  /*************************************************** 
  This is an example for the Adafruit Thermocouple Sensor w/MAX31855K

  Designed specifically to work with the Adafruit Thermocouple Sensor
  ----> https://www.adafruit.com/products/269

  These displays use SPI to communicate, 3 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_MAX31855.h"
#include <Time.h>  

//#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
//#define TIME_HEADER  'T'   // Header tag for serial time sync message
//#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

int thermoCLK =4 ;
int thermoCS = 5;
int thermoDO = 6;

// Initialize the Thermocouple
Adafruit_MAX31855 thermocouple(thermoCLK, thermoCS, thermoDO);


  
void setup() {
  Serial.begin(9600);

}

void loop() {

   Serial.println(thermocouple.readCelsius()); 
   //Serial.println(hour());
   //Serial.println(minute());
   //Serial.println(second());


   delay(1000);
}
