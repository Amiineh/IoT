#include <FastLED.h>

#define LED_PIN 13
#define ANALOG_READ 0
#define NUM_LEDS 120

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);

  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(255, 0, 0);
    }

  Serial.begin(9600);
  FastLED.show();
}

void loop() {
  int sensor_val = analogRead(ANALOG_READ);
  int show_leds = map(sensor_val, 0, 1023, 0, NUM_LEDS+10);
  Serial.println(show_leds);

  for(int i = 0; i < NUM_LEDS; i++){
    if (i < show_leds){
      leds[i] = CRGB(0, 255, 0);
      } else {
        leds[i] = CRGB(0, 0, 0);
        }
    }
    FastLED.show();
  delay(5);
}
