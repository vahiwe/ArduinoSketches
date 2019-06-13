#include <DS3231.h>
// Code from the Demo Example of the DS3231 Library
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid     = "virus";
const char *password = "";

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", utcOffsetInSeconds);

DS3231  rtc(SDA, SCL);

void setup()
{
  // Setup Serial connection
  Serial.begin(115200);
  // Uncomment the next line if you are using an Arduino Leonardo
  //while (!Serial) {}
    WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
  // Initialize the rtc object
  rtc.begin();

  timeClient.update();
  // The following lines can be uncommented to set the date and time
  rtc.setDOW(SUNDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime((int)timeClient.getHours(), (int)timeClient.getMinutes(), (int)timeClient.getSeconds());     // Set the time to 12:00:00 (24hr format)
  rtc.setTime(14, 27, 40);
  rtc.setDate(9, 6, 2019);   // Set the date to January 1st, 2014
}
void loop() {
  // put your main code here, to run repeatedly:

}
