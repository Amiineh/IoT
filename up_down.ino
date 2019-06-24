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
const int stay_b = 5; // change bases
int change_b = 0;
int k = 0; // rainbow color bias
const int stay_k = 3;  // last more on a color
int change_k = 0;

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
    fade_to_white(i, Scroll((i+k)*15 % 256), ons[i]);
  }
  
  for(int i = 0; i < WIDTH; i++){
    ons[i] = base[i];
    }

  change_k++;
  if (change_k == stay_k){
      k = (k+1) % 256;
      change_k = 0;
    }

  change_b++;
  if (change_b == stay_b){
    for(int i = 0; i < WIDTH; i++){
      ons[i] = base[i] = random(6, 12);
      }
      change_b = 0;
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


// rainbow colors
CRGB Scroll(int pos) {
  CRGB color (0,0,0);
  if(pos < 85) {
    color.g = 0;
    color.r = ((float)pos / 85.0f) * 255.0f;
    color.b = 255 - color.r;
  } else if(pos < 170) {
    color.g = ((float)(pos - 85) / 85.0f) * 255.0f;
    color.r = 255 - color.g;
    color.b = 0;
  } else if(pos < 256) {
    color.b = ((float)(pos - 170) / 85.0f) * 255.0f;
    color.g = 255 - color.b;
    color.r = 1;
  }
  return color;
}
