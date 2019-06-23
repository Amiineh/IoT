#include <FastLED.h>
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

const int ANALOG_READ = A0;
const int LED_PIN = 13;
const int NUM_LEDS = 120;
const int BRIGHTNESS = 60;
const int HEIGHT = 12;
const int WIDTH = 10;
CRGB leds[NUM_LEDS];
int base[WIDTH];
int ons[WIDTH];

CRGB Red = CRGB(255,0,0);
CRGB Green = CRGB(0,255,0);
CRGB Blue = CRGB(0,0,255);
CRGB Yellow = CRGB(255,255,0);
CRGB Purple = CRGB(178, 0, 255);
CRGB Lightblue = CRGB(0, 255, 255);
CRGB Pink = CRGB( 255, 43, 148);
CRGB Orange = CRGB( 255, 153, 0);

CRGB colors[100] = {Red, Orange, Yellow, Purple, Lightblue, Pink, Blue, Green};


void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);

  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(255, 0, 0);
    }

  for(int i = 0; i < WIDTH; i++){
    ons[i] = base[i] = random(6, 12);
    }

  Serial.begin(9600);
  FastLED.show();
}


void loop() {
  int audio_input = analogRead(ANALOG_READ); // ADD x2 HERE FOR MORE SENSITIVITY  
  
  for(int i = 0; i < WIDTH; i++){
    ons[i] += (1-base[i]) * audio_input / 1023;
    }

  for (int i = 0; i < WIDTH; i++){
    fade_to_white(i, colors[i], ons[i]);
  }
  
  for(int i = 0; i < WIDTH; i++){
    ons[i] = base[i];
    }

}


int get_idx(int x, int y){
  return x * HEIGHT + y;
  }


void fade_to_white(int col_idx, CRGB curr, int len){
  for (int i = 0; i < HEIGHT; i++){
    int idx = get_idx(col_idx, i);
    if (i < len){
      leds[idx].r = i * (255 - curr.r)/11 + curr.r;
      leds[idx].g = i * (255 - curr.g)/11 + curr.g;
      leds[idx].b = i * (255 - curr.b)/11 + curr.b;
      }
    else
      leds[idx] = CRGB(0, 0, 0);
    }
    FastLED.show();
  }
