#include <Wire.h>
#include <LiquidCrystal.h>
#include <FastLED.h>
#include "RTClib.h"

RTC_DS3231 rtc;
DateTime now;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

#define LED_TYPE WS2811


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int NUM_LEDS = 120;
const int HEIGHT = 12;
const int DATA_PIN = 13;
const int BRIGHTNESS = 55;
CRGB leds[NUM_LEDS];

const int nums[10] = {31599, 9362, 29671, 29391, 23497, 31183, 18927, 29257, 31727, 31689};

struct point{
  int x; 
  int y;
  point(int a, int b){
    x = a; y = b;
  }
}; 

point o_h1(1, 11);
point o_h2(5, 11);
point o_m1(1, 5);
point o_m2(5, 5);

void setup ()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  
  FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC Module");
    while (1);
  }

  if (rtc.lostPower()) 
  {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  delay(2000);
}


void loop () 
{
  now = rtc.now();
  showDate();
  showDay();
  showTime();
  viewClock(now.hour(), now.minute());
}


void viewNum(int t, point o){
  int cnt = 14;
  for(int j = 0; j < 5; j++){
    for(int i = 0; i < 3; i++){
        int idx = (o.x + i) * HEIGHT + o.y - j;
        bool on = (nums[t] >> cnt) & 1;
        if (on){
//          leds[idx] = 0x7FFF00;
          leds[idx] = CRGB::HotPink;
        }
        else
          leds[idx] = 0x000000;
        cnt--;
      } 
    }
    FastLED.show();
}


void viewClock(int h, int m){
  viewNum(h/10, o_h1);
  viewNum(h%10, o_h2);
  viewNum(m/10, o_m1);
  viewNum(m%10, o_m2);
}


void showDate()
 {
  lcd.setCursor(0,0);
  lcd.print(now.day());
  lcd.print('/');
  lcd.print(now.month());
  lcd.print('/');
  lcd.print(now.year());
 }

 
void showDay()
 {
  lcd.setCursor(11,0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
 }


void showTime()
 {
  lcd.setCursor(0,1);
  lcd.print("Time:");
  lcd.print(now.hour());
  lcd.print(':');
  lcd.print(now.minute());
  lcd.print(':');
  lcd.print(now.second());
  lcd.print("    ");
 }
