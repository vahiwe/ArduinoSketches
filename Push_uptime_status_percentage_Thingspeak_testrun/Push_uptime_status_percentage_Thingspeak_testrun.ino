#include <Wire.h>
#include "RTClib.h"
#include <ESP8266Ping.h>
#include "ThingSpeak.h"
#include "secrets.h"

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

RTC_DS3231 rtc;
bool ret;

#include <ESP8266WiFi.h>

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key index number (needed only for WEP)
WiFiClient  client;

const long utcOffsetInSeconds = 3600;
unsigned long starttime;
unsigned long networktime;
unsigned long connecttime;
unsigned long cdiff;
unsigned long ndiff;
unsigned long diff;
unsigned long cdiffstate;
unsigned long ndiffstate;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);


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
}

void loop() {

  if (cdiffstate == 0 && cdiff > 0) {
    cdiffstate += cdiff;
  }
  if (cdiff > cdiffstate) {
    cdiffstate = cdiff;
  }
  if (WiFi.status() != WL_CONNECTED) {
    if (networktime > 0 && ndiff > 0) {
      ndiffstate = ndiff;
      ndiff = 0;
      networktime = 0;
    }
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    if (networktime == 0) {
      DateTime now = rtc.now();
      networktime = now.unixtime();
    }
    Serial.println("\nConnected.");
    ret = Ping.ping("www.google.com");
    if (ret == true) {
      if (connecttime > 0 && cdiff > 0) {
        cdiffstate = cdiff;
      }
      if (connecttime == 0) {
        DateTime now = rtc.now();
        connecttime = now.unixtime();
      }
    }
    while (ret == false) {
      if (connecttime > 0 && cdiff > 0) {
        cdiffstate = cdiff;
        connecttime = 0;
        cdiff = 0;
      }
      Serial.println("No Internet Connection.....");
      ret = Ping.ping("www.google.com");
      if (ret == true) {
        if (connecttime > 0 && cdiff > 0) {
          cdiffstate = cdiff;
        }
        if (connecttime == 0) {
          DateTime now = rtc.now();
          connecttime = now.unixtime();
        }
      }
      if (WiFi.status() != WL_CONNECTED) {
        break;
      }
    }
  } else {
    if (networktime > 0 && ndiff > 0) {
      ndiffstate = ndiff;
    }
    if (networktime == 0) {
      DateTime now = rtc.now();
      networktime = now.unixtime();
    }
  }
  //    if (ret == true) {
  //      int avg_time_ms = Ping.averageTime();
  //      Serial.println(avg_time_ms);
  //    }

  ret = Ping.ping("www.google.com");
  if (ret == true) {
    Serial.print("T1---");
    Serial.print(cdiffstate);
    Serial.print("   T2----");
    Serial.println(cdiff);
    Serial.println();
    delay(20000);
    if (connecttime > 0 && cdiff > 0) {
      Serial.print("LTU1---");
      Serial.print(cdiffstate);
      Serial.print("   LTU2----");
      Serial.println(cdiff);
      Serial.println();
      delay(20000);
      cdiffstate = cdiff;
      Serial.print("LTD1---");
      Serial.print(cdiffstate);
      Serial.print("   LTD2----");
      Serial.println(cdiff);
      Serial.println();
      delay(20000);
    }
    if (connecttime == 0) {
      DateTime now = rtc.now();
      connecttime = now.unixtime();
      Serial.print("L2T1---");
      Serial.print(cdiffstate);
      Serial.print("   L2T2----");
      Serial.println(cdiff);
      Serial.println();
      delay(20000);
    }
  }
  while (ret == false) {
    if (connecttime > 0 && cdiff > 0) {
      Serial.print("F1---");
      Serial.print(cdiffstate);
      Serial.print("   F2----");
      Serial.println(cdiff);
      Serial.println();
      delay(20000);
      cdiffstate = cdiff;
      connecttime = 0;
      cdiff = 0;
      Serial.print("FC1---");
      Serial.print(cdiffstate);
      Serial.print("   FC2----");
      Serial.println(cdiff);
      Serial.println();
      delay(20000);
    }
    Serial.println("No Internet Connection");
    ret = Ping.ping("www.google.com");
    if (ret == true) {
      if (connecttime > 0 && cdiff > 0) {
        Serial.print("FT1---");
        Serial.print(cdiffstate);
        Serial.print("   FT2----");
        Serial.println(cdiff);
        Serial.println();
        delay(20000);
        cdiffstate = cdiff;
        Serial.print("FTC1---");
        Serial.print(cdiffstate);
        Serial.print("   FT2----");
        Serial.println(cdiff);
        Serial.println();
        delay(20000);
      }
      if (connecttime == 0) {
        DateTime now = rtc.now();
        connecttime = now.unixtime();
      }
    }
    if (WiFi.status() != WL_CONNECTED) {
      break;
    }
  }
  //  if (ret == true) {
  //    int avg_time_ms = Ping.averageTime();
  //    Serial.println(avg_time_ms);
  //  }
  // put your main code here, to run repeatedly:
  DateTime now = rtc.now();
  diff = now.unixtime() - starttime;
  //  Serial.print("Elapsed Time, Minutes:");
  //  Serial.print(diff / 60L);
  //  Serial.print("Elapsed Time, Seconds:");
  //  Serial.println(diff % 60L);
  //  Serial.println();
  //  Serial.println();
  ndiff = now.unixtime() - networktime;
  if (ndiff <= ndiffstate) {
    ndiff += ndiffstate;
  }
  //  Serial.print("network Time, Minutes:");
  //  Serial.print(ndiff / 60L);
  //  Serial.print("network Time, Seconds:");
  //  Serial.println(ndiff % 60L);
  //  Serial.println();
  //  Serial.println();
  cdiff = now.unixtime() - connecttime;
  Serial.print("L1---");
  Serial.print(cdiffstate);
  Serial.print("   L2----");
  Serial.println(cdiff);
  Serial.println();
  delay(20000);
  if (cdiff <= cdiffstate) {
    Serial.print("LU1---");
    Serial.print(cdiffstate);
    Serial.print("   LU2----");
    Serial.println(cdiff);
    Serial.println();
    delay(20000);
    cdiff += cdiffstate;
    Serial.print("LD1---");
    Serial.print(cdiffstate);
    Serial.print("   LD2----");
    Serial.println(cdiff);
    Serial.println();
    delay(20000);
  }
  Serial.print("connect Time, Minutes:");
  Serial.print(cdiff / 60L);
  Serial.print("connect Time, Seconds:");
  Serial.println(cdiff % 60L);
  Serial.println();
  Serial.println();

  //  int httpCode = ThingSpeak.writeField(myChannelNumber, 1, (long)diff, myWriteAPIKey);
  //int httpCode = ThingSpeak.writeField(myChannelNumber, 1, (long)now.unixtime(), myWriteAPIKey);
  Serial.println(now.unixtime(), DEC);
  //  if (httpCode == 200) {
  //    Serial.println("Channel write successful.");
  //  }
  //  else {
  //    Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
  //  }

  // Wait 20 seconds to uodate the channel again
  delay(10000);
}
