#include <DS3231.h>
//#include <LiquidCrystal.h>

DS3231  rtc(SDA, SCL);
//LiquidCrystal lcd(1, 2, 4, 5, 6, 7);
void setup() { 
 rtc.begin();
 Serial.begin(115200);
}
void loop() { 
 //lcd.setCursor(0,0);
 Serial.print("Time:  ");
 Serial.println(rtc.getTimeStr());
 
 //lcd.setCursor(0,1);
 Serial.print("Date: ");
 Serial.println(rtc.getDateStr());
 
 delay(1000); 
}
