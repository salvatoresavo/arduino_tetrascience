/*************************************************** 
  This is a sketch to use the CC3000 WiFi chip & Xively
  
  Written by Marco Schwartz for Open Home Automation
 ****************************************************/

// Libraries
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
#include<stdlib.h>
#include "Adafruit_MAX31855.h"
#include <Time.h>  
#include <avr/wdt.h>

// Define CC3000 chip pins
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// thermocouple
int thermoCLK = 2;
int thermoCS = 6;
int thermoDO = 7;

// Thermocouple instances
Adafruit_MAX31855 thermocouple(thermoCLK, thermoCS, thermoDO);

// Create CC3000 instances
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2); // you can change this clock speed
                                         

// WLAN parameters
//#define WLAN_SSID       "Starlight2"
//#define WLAN_PASS       "B5F7200FAAB4E8D3C4AD7D6B"
//#define WLAN_SECURITY   WLAN_SEC_WPA2

//#define WLAN_SSID       "Rowland-2C"
//#define WLAN_PASS       "abcde12345"
//#define WLAN_SECURITY   WLAN_SEC_WPA2


#define WLAN_SSID       "HBSGUEST"
#define WLAN_SECURITY   WLAN_SEC_UNSEC


//#define WLAN_SSID       "Harvard University"
////#define WLAN_PASS       "60910690" // Sal's Harvard pin
#define WLAN_PASS       "90881973"   // Alok's pin
//#define WLAN_SECURITY   WLAN_SEC_UNSEC

//#define WLAN_SSID       "Stegasaurus"
//#define WLAN_PASS       "ravenscrest"
//#define WLAN_SECURITY   WLAN_SEC_WPA2

// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
//#define WLAN_SECURITY   WLAN_SEC_WPA2

// Xively parameters
#define WEBSITE  "api.xively.com"
// PRIVATE 
//#define API_key  "vJkYN0UjSYbyqs6YOn6SlnprAHcTXZEqDUsVJdTzu7OUP0bn"
//#define feedID  "2028754341"
// PUBLIC
//TC1
#define API_key  "PFZ9sH07dNZENHDFczBa8q3jYcQGYq5zcPKmM7qMvgJPxUIS"
#define feedID  "1262643534"

//TC2
//#define API_key  "JU9aF9Khovrqu6VIT4ZkReOyWvS1uzfrFQnejTGdcXosTPxr"
//#define feedID  "1295198993"

//#define API_key  "BdcR3nEo7J6sUV9X4Xqr8CgKBXPqC61231qPl5AQeorgfSpv"
//#define feedID  "1059837945"

//#define API_key  "c4WPipK8doxrllEM8mS1vizxlb5sHFgS0Zik1fTrvSMtOm3l"
//#define feedID  "631602878"

#define IDLE_TIMEOUT_MS 100000
#define FAIL_MAX 5
uint32_t ip;

void setup(void)
{
  // Initialize
  Serial.begin(115200);
  
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }
  
  wdt_disable();
  wdt_enable(WDTO_8S);
 
}

void loop(void)
{
  
  Serial.print(F("\nbegin: "));
  display_time();
  
  
  /* ------ Connect to WiFi network -------*/
  Serial.println(F("Connecting to WIFI..."));
  wdt_reset();
  cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  Serial.println(F("Connected to WIFI!"));
  display_time();

  
  /*--------- Request DHCP -------------- */
  // Wait for it to complete 
  // Or force system reboot
  wdt_reset();
  unsigned long lastRead_DHCP = millis();
  Serial.println(F("Requesting DHCP..."));
  while (!cc3000.checkDHCP() && (millis() - lastRead_DHCP < IDLE_TIMEOUT_MS))
  {
    delay(100);
  }  
  if ((millis()-100-lastRead_DHCP) > IDLE_TIMEOUT_MS){
    Serial.println(F("Reboot due to DHCP request time out!"));
    software_Reboot();
  }  
  Serial.println(F("DHCP Requested"));
  
  
  /*----------Get the website IP & print it-------*/
  wdt_reset();
  ip = 0;
  unsigned long num_fail = 0;
  Serial.print(WEBSITE); Serial.print(F(" -> "));
  while (ip == 0) {
    if (num_fail > FAIL_MAX){
      Serial.println(F("Reboot due to >5 unresolve!"));
      software_Reboot();}
    if (! cc3000.getHostByName(WEBSITE, &ip)) {
      Serial.println(F("Couldn't resolve!"));
    }
    num_fail = num_fail + 1;
    delay(200);
  }
  cc3000.printIPdotsRev(ip);
  Serial.println();
  
  /* -----Get data & convert to integers ---------*/
  wdt_reset();
  float h = thermocouple.readCelsius();
  float t = thermocouple.readCelsius();  
//  int temperature = (int) t;
  int humidity = (int) h;
  
   char tmp[10];
  
  String temperature =dtostrf(t,1,2,tmp);
  
  // display temp data from the sensor
  Serial.println("Measured Temperature");
  Serial.println(temperature);
  
  // Prepare JSON for Xively & get length
  int length = 0;

  String data = "";
  data = data + "\n" + "{\"version\":\"1.0.0\",\"datastreams\" : [ {\"id\" : \"Temperature\",\"current_value\" : \"" + String(temperature) + "\"}]}"; 
  //data = data + "\n" + "{\"version\":\"1.0.0\",\"datastreams\" : [ {\"id\" : \"Temperature\",\"current_value\" : \"" + String(temperature) + "\"}]}"; 
  //+ "{\"id\" : \"Humidity\",\"current_value\" : \"" + String(humidity) + "\"}]}";
  
 // data = String("name=whitside_tc_1&temp=" + String(temperature));
  
  length = data.length();
  //Serial.println();
  Serial.print("\nData length ");
  Serial.println(length);
  Serial.println();

  
  /* -----------Post data to the webserver----------- */
  wdt_reset();
  Adafruit_CC3000_Client client = cc3000.connectTCP(ip, 80);
  wdt_reset();
  if (client.connected()) {
      Serial.println("Posting to Webserver ...");
      wdt_reset();
      client.println("PUT /v2/feeds/" + String(feedID) + ".json HTTP/1.0");
      client.println("Host: api.xively.com");
      wdt_reset();
      client.println("X-ApiKey: " + String(API_key));
      client.println("Content-Length: " + String(length));
      wdt_reset();
      client.print("Connection: close");
      client.println();
      client.print(data);
      client.println();
      Serial.println("Posting to Webserver done.");
      wdt_reset();
      
      //client.println();
  } else {
    Serial.println(F("Connection failed"));    
    return;
  }
  
  
  /*------ Print Server Response ------------*/ 
  wdt_reset();
  unsigned long lastRead = millis();
  Serial.println(F("--------------Server Response---------------- \n"));
  while (client.connected() && (millis()-lastRead < IDLE_TIMEOUT_MS)) {
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
      lastRead = millis();
     }
  }
  if ((millis()-lastRead) > IDLE_TIMEOUT_MS){
    Serial.println(F("Reboot due to server response time out!"));
    software_Reboot();
  }
 Serial.println(F("--------------Server Response----------------"));
 
 /*---------Disconnect from Internet---------------*/
 wdt_reset();
 client.close();
 // client.stop();  
 Serial.println(F("Disconnecting...."));
 cc3000.disconnect();  
 Serial.print(F("end: "));
 display_time();
 delay(500);
  
}

// Display elapsed time
void display_time(){
   Serial.print(hour());
   Serial.print(F(":"));
   Serial.print(minute());
   Serial.print(F(":"));
   Serial.println(second());
}

// force system reboot using watch dog
void software_Reboot(){
  wdt_enable(WDTO_15MS);
  while(1){}
}

