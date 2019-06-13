#include <Wire.h>
#include "RTClib.h"
#include <ESP8266Ping.h>
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include "secrets.h"

unsigned long myChannelNumber = SECRET_CH_ID;
unsigned int WiFiConnectionTime = 1;
unsigned int ConnectionTimeStatus = 2;
unsigned int StartTimeVal = 3;
unsigned int RuntimeStatus = 4;
unsigned int LastSeen = 5;
unsigned int OffTimeStatus = 6;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

RTC_DS3231 rtc;
bool ret;

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key index number (needed only for WEP)
WiFiClient  client;

const long utcOffsetInSeconds = 3600;
unsigned long starttime;
long firststarttime;
unsigned long range;
long oldstarttime;
int offtimepercent;
int ontimepercent;
unsigned long endtime;
unsigned long networktime;
unsigned long connecttime;
unsigned long cdiff;
unsigned long ndiff;
unsigned long diff;
long diffstate;
unsigned long cdiffstate;
unsigned long ndiffstate;
unsigned long ness;
unsigned long pol;
int cdiffpercent;
int ndiffpercent;
int statusCode;
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
  diffstate = 0;
  cdiffstate = 0;
  ndiffstate = 0;
  endtime = 0;

  if (WiFi.status() != WL_CONNECTED) {
    ndiff = 0;
    networktime = 0;
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network
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
      connecttime = 0;
      cdiff = 0;
      if (connecttime == 0) {
        DateTime now = rtc.now();
        connecttime = now.unixtime();
      }
    }
    while (ret == false) {
      connecttime = 0;
      cdiff = 0;
      Serial.println("No Internet Connection.....");
      ret = Ping.ping("www.google.com");
      if (ret == true) {
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
    if (networktime == 0) {
      DateTime now = rtc.now();
      networktime = now.unixtime();
    }
  }

  firststarttime = starttime;
  oldstarttime = ThingSpeak.readLongField(myChannelNumber, StartTimeVal);
  statusCode = ThingSpeak.getLastReadStatus();
  while (statusCode != 200) {
    Serial.println(statusCode);
    oldstarttime = ThingSpeak.readLongField(myChannelNumber, StartTimeVal);
    statusCode = ThingSpeak.getLastReadStatus();
  }

  if (oldstarttime == -1) {
    Serial.println("Condition 1: First Run");
    ThingSpeak.setField(StartTimeVal, firststarttime);
    int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    while (httpCode != 200) {
      ThingSpeak.setField(StartTimeVal, firststarttime);
      int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    }
  } else {
    Serial.println("Condition 2: Rerun");
    firststarttime = oldstarttime;

    cdiffpercent = ThingSpeak.readIntField(myChannelNumber, ConnectionTimeStatus);
    statusCode = ThingSpeak.getLastReadStatus();
    if (cdiffpercent == -1) {
      cdiffpercent = 0;
    }
    while (statusCode != 200) {
      cdiffpercent = ThingSpeak.readIntField(myChannelNumber, ConnectionTimeStatus);
      statusCode = ThingSpeak.getLastReadStatus();
      if (cdiffpercent == -1) {
        cdiffpercent = 0;
      }
    }
    //cdiffstate =

    ndiffpercent = ThingSpeak.readIntField(myChannelNumber, WiFiConnectionTime);
    statusCode = ThingSpeak.getLastReadStatus();
    if (ndiffpercent == -1) {
      ndiffpercent = 0;
    }
    while (statusCode != 200) {
      ndiffpercent = ThingSpeak.readIntField(myChannelNumber, WiFiConnectionTime);
      statusCode = ThingSpeak.getLastReadStatus();
      if (ndiffpercent == -1) {
        ndiffpercent = 0;
      }
    }
    //ndiffstate = ndiffpercent * diff

    diffstate = ThingSpeak.readLongField(myChannelNumber, RuntimeStatus);
    statusCode = ThingSpeak.getLastReadStatus();
    if (diffstate == -1) {
      diffstate = 0;
    }
    while (statusCode != 200) {
      diffstate = ThingSpeak.readLongField(myChannelNumber, RuntimeStatus);
      statusCode = ThingSpeak.getLastReadStatus();
      if (diffstate == -1) {
        diffstate = 0;
      }
    }

    endtime = ThingSpeak.readLongField(myChannelNumber, LastSeen);
    statusCode = ThingSpeak.getLastReadStatus();
    if (endtime == -1) {
      endtime = 0;
    }
    while (statusCode != 200) {
      endtime = ThingSpeak.readLongField(myChannelNumber, LastSeen);
      statusCode = ThingSpeak.getLastReadStatus();
      if (endtime == -1) {
        endtime = 0;
      }
    }
    range = endtime - firststarttime;
    cdiffstate = (cdiffpercent * diffstate) / 100L;
    ndiffstate = (ndiffpercent * diffstate) / 100L;
    ontimepercent = (int)((diffstate * 100L) / range);
    offtimepercent = 100 - ontimepercent;
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    ndiff = 0;
    networktime = 0;
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network
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
      connecttime = 0;
      cdiff = 0;
      if (connecttime == 0) {
        DateTime now = rtc.now();
        connecttime = now.unixtime();
      }
    }
    while (ret == false) {
      connecttime = 0;
      cdiff = 0;
      Serial.println("No Internet Connection.....");
      ret = Ping.ping("www.google.com");
      if (ret == true) {
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
    if (networktime == 0) {
      DateTime now = rtc.now();
      networktime = now.unixtime();
    }
  }

  ret = Ping.ping("www.google.com");
  if (ret == true) {
    if (connecttime == 0) {
      DateTime now = rtc.now();
      connecttime = now.unixtime();
    }
  }
  while (ret == false) {
    connecttime = 0;
    cdiff = 0;
    Serial.println("No Internet Connection");
    ret = Ping.ping("www.google.com");
    if (ret == true) {
      if (connecttime == 0) {
        DateTime now = rtc.now();
        connecttime = now.unixtime();
      }
    }
    if (WiFi.status() != WL_CONNECTED) {
      break;
    }
  }


  DateTime now = rtc.now();
  long present = now.unixtime();
  if (starttime != 0) {
    ness = diff;
    diff = present - starttime;
    pol = diff - ness;
  }
  diffstate += pol;
  //diff = now.unixtime() - starttime;
  Serial.print("Present Runtime, \nMinutes:");
  Serial.print(diff / 60L);
  Serial.print(" Seconds:");
  Serial.println(diff % 60L);
  Serial.println();
  if (networktime != 0) {
    ness = ndiff;
    ndiff = present - networktime;
    pol = ndiff - ness;
  }
  ndiffstate += pol;
  Serial.print("Present Wi-Fi Connection Time Since Last Disconnection, \nMinutes:");
  Serial.print(ndiff / 60L);
  Serial.print(" Seconds:");
  Serial.println(ndiff % 60L);
  Serial.println((unsigned long)pol);
  Serial.println((unsigned long)ndiffstate);
  Serial.println();
  if (connecttime != 0) {
    ness = cdiff;
    cdiff = present - connecttime;
    pol = cdiff - ness;
  }
  cdiffstate += pol;
  Serial.print("Present Internet Connection Time Since Last Disconnection, \nMinutes:");
  Serial.print(cdiff / 60L);
  Serial.print(" Seconds:");
  Serial.println(cdiff % 60L);
  Serial.println((unsigned long)pol);
  Serial.println((unsigned long)cdiffstate);
  Serial.println();

  cdiffpercent = (int)((cdiffstate * 100L) / diffstate);
  ndiffpercent = (int)((ndiffstate * 100L) / diffstate);
  range = present - firststarttime;
  ontimepercent = (int)((diffstate * 100L) / range);
  offtimepercent = 100 - ontimepercent;
  Serial.print("uptime status: ");
  Serial.print(ndiffpercent);
  Serial.println("%");
  Serial.print("connection status: ");
  Serial.print(cdiffpercent);
  Serial.println("%");

  ThingSpeak.setField(WiFiConnectionTime, ndiffpercent);
  ThingSpeak.setField(ConnectionTimeStatus, cdiffpercent);
  ThingSpeak.setField(RuntimeStatus, diffstate);
  ThingSpeak.setField(LastSeen, present);
  ThingSpeak.setField(OffTimeStatus, offtimepercent);

  //ThingSpeak.setField(3, p);
  //ThingSpeak.setField(4, pt);
  int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  //  int httpCode = ThingSpeak.writeField(myChannelNumber, 1, ndiffpercent, myWriteAPIKey);
  //  int httpCode2 = ThingSpeak.writeField(myChannelNumber, 2, cdiffpercent, myWriteAPIKey);
  //int httpCode = ThingSpeak.writeField(myChannelNumber, 1, (long)now.unixtime(), myWriteAPIKey);
  //Serial.println(now.unixtime(), DEC);
  if (httpCode == 200) {
    Serial.println("Channel write successful.");
  }
  else {
    Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
  }
  //    if (httpCode2 == 200) {
  //      Serial.println("Channel write successful.");
  //    }
  //    else {
  //      Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
  //    }

  // Wait 20 seconds to uodate the channel again
  delay(10000);
}
