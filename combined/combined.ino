#include <Wire.h>
#include <LiquidCrystal.h>
#include "RTClib.h"
#include <FastLED.h>


/*    -------------------------------LEDS-------------------------------    */
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

const int LED_PIN = 13;
const int NUM_LEDS = 120;
const int BRIGHTNESS = 60;
const int HEIGHT = 12;
CRGB leds[NUM_LEDS];


/*    ------------------------------BUTTON-------------------------------    */
const int  BUTTON_PIN = 2;
const int MAX_STATES = 3;
enum state {clk, paint, dance};
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button


/*    -------------------------------DANCE-------------------------------    */
const int UPDATES_PER_SECOND = 100;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


/*    -------------------------------CLOCK-------------------------------    */
RTC_DS3231 rtc;
DateTime now;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
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


/*    --------------------------------LCD--------------------------------    */
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {
  delay( 3000 ); // power-up safety delay
  
  // LEDS
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  // BUTTON
  pinMode(BUTTON_PIN, INPUT);

  // DANCE
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  // CLOCK
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


  // LCD
  lcd.begin(16,2);
  Serial.begin(9600);
}


void loop() {
  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      buttonPushCounter++;
      clear_leds();
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(buttonPushCounter);
    } else {
      Serial.println("off");
    }
    delay(50);
  }
  lastButtonState = buttonState;


  state current = static_cast<state>(buttonPushCounter % MAX_STATES);
  switch (current){
    case clk:
      run_clock();
      break;
    case paint:
      run_paint();
      break;
    case dance:
      run_dance();
      break;
    }
}



/*    
╱╱╱╱╱╱╱╱╭━━━╮╭╮╱╱╱╭━━━╮╭━━━╮╭╮╭━╮╱╱╱╱╱╱╭╮
╱╱╱╱╱╱╱╱┃╭━╮┃┃┃╱╱╱┃╭━╮┃┃╭━╮┃┃┃┃╭╯╱╱╱╱╱╱┃┃
╱╱╱╱╱╱╱╱┃┃╱╰╯┃┃╱╱╱┃┃╱┃┃┃┃╱╰╯┃╰╯╯╱╱╱╱╱╱╱┃┃
╱╱╱╱╱╱╱╱┃┃╱╭╮┃┃╱╭╮┃┃╱┃┃┃┃╱╭╮┃╭╮┃╱╱╱╱╱╱╱╰╯
╭╮╭╮╭╮╭╮┃╰━╯┃┃╰━╯┃┃╰━╯┃┃╰━╯┃┃┃┃╰╮╭╮╭╮╭╮╭╮
╰╯╰╯╰╯╰╯╰━━━╯╰━━━╯╰━━━╯╰━━━╯╰╯╰━╯╰╯╰╯╰╯╰╯
*/
void run_clock(){
  Serial.println("clk");
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


/*
╱╱╱╱╱╱╱╱╭━━━╮╭━━━╮╭━━╮╭━╮╱╭╮╭━━━━╮╱╱╱╱╱╱╭╮
╱╱╱╱╱╱╱╱┃╭━╮┃┃╭━╮┃╰┫┣╯┃┃╰╮┃┃┃╭╮╭╮┃╱╱╱╱╱╱┃┃
╱╱╱╱╱╱╱╱┃╰━╯┃┃┃╱┃┃╱┃┃╱┃╭╮╰╯┃╰╯┃┃╰╯╱╱╱╱╱╱┃┃
╱╱╱╱╱╱╱╱┃╭━━╯┃╰━╯┃╱┃┃╱┃┃╰╮┃┃╱╱┃┃╱╱╱╱╱╱╱╱╰╯
╭╮╭╮╭╮╭╮┃┃╱╱╱┃╭━╮┃╭┫┣╮┃┃╱┃┃┃╱╱┃┃╱╱╭╮╭╮╭╮╭╮
╰╯╰╯╰╯╰╯╰╯╱╱╱╰╯╱╰╯╰━━╯╰╯╱╰━╯╱╱╰╯╱╱╰╯╰╯╰╯╰╯

 */
 
void run_paint(){
  Serial.println("paint");
}



/* 
╱╱╱╱╱╱╱╱╭━━━╮╭━━━╮╭━╮╱╭╮╭━━━╮╭━━━╮╱╱╱╱╱╱╭╮
╱╱╱╱╱╱╱╱╰╮╭╮┃┃╭━╮┃┃┃╰╮┃┃┃╭━╮┃┃╭━━╯╱╱╱╱╱╱┃┃
╱╱╱╱╱╱╱╱╱┃┃┃┃┃┃╱┃┃┃╭╮╰╯┃┃┃╱╰╯┃╰━━╮╱╱╱╱╱╱┃┃
╱╱╱╱╱╱╱╱╱┃┃┃┃┃╰━╯┃┃┃╰╮┃┃┃┃╱╭╮┃╭━━╯╱╱╱╱╱╱╰╯
╭╮╭╮╭╮╭╮╭╯╰╯┃┃╭━╮┃┃┃╱┃┃┃┃╰━╯┃┃╰━━╮╭╮╭╮╭╮╭╮
╰╯╰╯╰╯╰╯╰━━━╯╰╯╱╰╯╰╯╱╰━╯╰━━━╯╰━━━╯╰╯╰╯╰╯╰╯
*/

void run_dance(){
  Serial.println("dance");
  ChangePalettePeriodically();
    
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  
  FillLEDsFromPaletteColors( startIndex);
  
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}


void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};


/* 
╱╱╱╱╱╱╱╱╭━━━╮╭╮╱╱╱╭━━━╮╭━━━╮╭━━━╮╱╱╱╱╱╱╭╮
╱╱╱╱╱╱╱╱┃╭━╮┃┃┃╱╱╱┃╭━━╯┃╭━╮┃┃╭━╮┃╱╱╱╱╱╱┃┃
╱╱╱╱╱╱╱╱┃┃╱╰╯┃┃╱╱╱┃╰━━╮┃┃╱┃┃┃╰━╯┃╱╱╱╱╱╱┃┃
╱╱╱╱╱╱╱╱┃┃╱╭╮┃┃╱╭╮┃╭━━╯┃╰━╯┃┃╭╮╭╯╱╱╱╱╱╱╰╯
╭╮╭╮╭╮╭╮┃╰━╯┃┃╰━╯┃┃╰━━╮┃╭━╮┃┃┃┃╰╮╭╮╭╮╭╮╭╮
╰╯╰╯╰╯╰╯╰━━━╯╰━━━╯╰━━━╯╰╯╱╰╯╰╯╰━╯╰╯╰╯╰╯╰╯
*/
void clear_leds(){
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Black;  
  }  
  FastLED.show();
} 
