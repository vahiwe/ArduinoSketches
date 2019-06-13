#include <Time.h>
#include <TimeLib.h>

#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
unsigned long starttime;
int i = 0;

void setup() {
  Serial.begin(115200);
  //delay(3000); // wait for console opening
  
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
  
}

void loop() {
    DateTime now = rtc.now();
//    i++;
//    Serial.println("Current Date & Time: ");
//    Serial.print(now.year(), DEC);
//    Serial.print('/');
//    Serial.print(now.month(), DEC);
//    Serial.print('/');
//    Serial.print(now.day(), DEC);
//    Serial.print(" (");
//    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
//    Serial.print(") ");
//    Serial.print(now.hour(), DEC);
//    Serial.print(':');
//    Serial.print(now.minute(), DEC);
//    Serial.print(':');
//    Serial.print(now.second(), DEC);
//    Serial.println();
//    Serial.print(" since midnight 1/1/1970 = ");
//    Serial.print(now.unixtime());
//    Serial.print("s = ");
//    Serial.print(now.unixtime() / 86400L);
//    Serial.println("d");
    long diff = now.unixtime() - starttime;
    Serial.print("Elapsed Time, Minutes:");
    Serial.print(diff / 60L);
    Serial.print("Elapsed Time, Seconds:");
    Serial.println(diff % 60L);
    Serial.println();
    Serial.println();
//      Serial.print("Elapsed Time, Hours:");
//      Serial.print(diff / 3600L);
//      Serial.print("Elapsed Time, Days:");
//      Serial.print(diff / 86400L);    
    delay(1000);
}
