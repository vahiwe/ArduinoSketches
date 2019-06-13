//#include "ThingSpeak.h"
//#include "secrets.h"
//#include <WiFi.h>
#include <Wire.h>
#include "RTClib.h"
#include <ESP8266Ping.h>
#include "ThingSpeak.h"
#include "secrets.h"
#include <ESP8266WiFi.h>

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

RTC_DS3231 rtc;
bool ret;

// Weather station channel details
unsigned long myChannelNumber = SECRET_CH_ID;
unsigned int WiFiConnectionTime = 1;
unsigned int ConnectionTimeStatus = 2;
unsigned int StartTimeVal = 3;
unsigned int RuntimeStatus = 4;
unsigned int LastSeen = 5;
unsigned int OffTimeStatus = 6;


// Counting channel details
//unsigned long counterChannelNumber = SECRET_CH_ID_COUNTER;
//const char * myCounterReadAPIKey = SECRET_READ_APIKEY_COUNTER;

const long utcOffsetInSeconds = 3600;
unsigned long starttime;
long rtl;
unsigned long rtu;
unsigned long networktime;
unsigned long connecttime;
unsigned long cdiff;
unsigned long ndiff;
//unsigned long diff;
unsigned long cdiffstate;
unsigned long ndiffstate;
unsigned long ness;
unsigned long pol;
int cdiffpercent;
int ndiffpercent;
String diff;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


void setup() {
  Serial.begin(115200);  //Initialize serial

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");

    // Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    // Following line sets the RTC with an explicit date & time
    // for example to set January 27 2017 at 12:56 you would call:
    //rtc.adjust(DateTime(2019, 6, 9, timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds()));
  }
  DateTime now = rtc.now();

  starttime = now.unixtime();
  networktime = 0;
  connecttime = 0;
  cdiffstate = 0;
  ndiffstate = 0;

  int statusCode = 0;
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected");
  }

  cdiffpercent = ThingSpeak.readIntField(myChannelNumber, ConnectionTimeStatus);
  Serial.print(cdiffpercent);

  // Check the status of the read operation to see if it was successful
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200) {
    Serial.println("Internet Connectivity Status: " + String(cdiffpercent) + "%");
  }
  else {
    Serial.println("Problem reading channel. HTTP error code " + String(statusCode));
  }

  ndiffpercent = ThingSpeak.readIntField(myChannelNumber, WiFiConnectionTime);
  Serial.print(ndiffpercent);

  // Check the status of the read operation to see if it was successful
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200) {
    Serial.println("Internet Connectivity Status: " + String(ndiffpercent) + "%");
  }
  else {
    Serial.println("Problem reading channel. HTTP error code " + String(statusCode));
  }

  diff = ThingSpeak.readStringField(myChannelNumber, StartTimeVal);
  Serial.print(diff);
  // Check the status of the read operation to see if it was successful
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200) {
    Serial.println("WiFi connectivity Status: " + String(diff) + "%");
    if (diff == "-1") {
      Serial.println("JoJo");
    }
  }
  else {
    Serial.println("Problem reading channel. HTTP error code " + String(statusCode));
  }



  delay(1000); // No need to read the temperature too often.

}

void loop() {
  DateTime now = rtc.now();

  rtl = now.unixtime();
  Serial.println(rtl);

  rtu = now.unixtime();
  Serial.println(rtu);

  Serial.println(rtu - rtl);
  delay(10000);

  //int statusCode = 0;

  // Connect or reconnect to WiFi
  //  if (WiFi.status() != WL_CONNECTED) {
  //    Serial.print("Attempting to connect to SSID: ");
  //    Serial.println(SECRET_SSID);
  //    while (WiFi.status() != WL_CONNECTED) {
  //      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
  //      Serial.print(".");
  //      delay(5000);
  //    }
  //    Serial.println("\nConnected");
  //  }

  // Read in field 4 of the public channel recording the temperature
  //  float temperatureInF = ThingSpeak.readFloatField(weatherStationChannelNumber, temperatureFieldNumber);

  // Check the status of the read operation to see if it was successful
  //  statusCode = ThingSpeak.getLastReadStatus();
  //  if (statusCode == 200) {
  //    Serial.println("Temperature at MathWorks HQ: " + String(temperatureInF) + " deg F");
  //  }
  //  else {
  //    Serial.println("Problem reading channel. HTTP error code " + String(statusCode));
  //  }

  //  delay(15000); // No need to read the temperature too often.

  // Read in field 1 of the private channel which is a counter
  //  long count = ThingSpeak.readLongField(counterChannelNumber, counterFieldNumber, myCounterReadAPIKey);

  // Check the status of the read operation to see if it was successful
  //  statusCode = ThingSpeak.getLastReadStatus();
  //  if (statusCode == 200) {
  //    Serial.println("Counter: " + String(count));
  //  }
  //  else {
  //    Serial.println("Problem reading channel. HTTP error code " + String(statusCode));
  //  }
  //
  //  delay(15000); // No need to read the counter too often.

}
