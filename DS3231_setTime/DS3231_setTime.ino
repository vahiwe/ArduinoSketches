#include <DS3231.h>
// Code from the Demo Example of the DS3231 Library
DS3231  rtc(SDA, SCL);
void setup()
{
  // Setup Serial connection
  Serial.begin(115200);
  // Uncomment the next line if you are using an Arduino Leonardo
  //while (!Serial) {}
  
  // Initialize the rtc object
  rtc.begin();
  
  // The following lines can be uncommented to set the date and time
  rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
  rtc.setTime(12, 0, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(1, 1, 2014);   // Set the date to January 1st, 2014
}
void loop() {
  // put your main code here, to run repeatedly:

}
