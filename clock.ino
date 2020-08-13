#include <FastLED.h>
#include <Wire.h>
#include <ds3231.h>

ts t;
#define LED_TYPE WS2811

const int NUM_LEDS = 60;
const int HEIGHT = 12;
const int DATA_PIN = 5;
const int BRIGHTNESS = 255;
CRGB leds[NUM_LEDS];

const int nums[10] = {31599, 9362, 29671, 29391, 23497, 31183, 18927, 29257, 31727, 31689};

struct point{
  int x; 
  int y;
  point(int a, int b){
    x = a; y = b;
  }
}; 

//point o_h1(1, 11);
//point o_h2(5, 11);
//point o_m1(1, 5);
//point o_m2(5, 5);

point o_h1(0, 11);
point o_h2(3, 11);
point o_m1(0, 5);
point o_m2(3, 5);

void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  
  Wire.begin(); //start i2c (required for connection)
  DS3231_init(DS3231_INTCN); //register the ds3231 (DS3231_INTCN is the default address of ds3231, this is set by macro for no performance loss)
  Serial.begin(9600);
  while(!Serial){} //only need for leonardo
}


void loop() {
  DS3231_get(&t);
  viewClock(t.min, t.sec);
}

void viewNum(int t, point o){
  int cnt = 14;
  for(int j = 0; j < 5; j++){
    for(int i = 0; i < 3; i++){
        int idx = (o.x + i) * HEIGHT + o.y - j;
        bool on = (nums[t] >> cnt) & 1;
        if (on)
          leds[idx] = CRGB::Blue;
        else
          leds[idx] = CRGB::Black;
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
