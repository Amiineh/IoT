
#include <Wire.h>
#include <LiquidCrystal.h>
#include "RTClib.h"
#include <FastLED.h>


/*    -------------------------------LEDS-------------------------------    */
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

const int LED_PIN = 13;
const int NUM_LEDS = 120;
const int BRIGHTNESS = 255;
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
  Serial.begin(115200);


  lcd.write("Awaiting");
  lcd.setCursor(0,1);
  lcd.write("Commands");
  lcd.setCursor(0,0);
  
  clear_leds();
}


void clear_leds(){
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Black;  
  }  
  FastLED.show();
} 


String data;

void loop() {
  lcd.clear();
  lcd.write("Awaiting");
  lcd.setCursor(0,1);
  lcd.write("Commands");
  lcd.setCursor(0,0);
  
  if(Serial.available()){
    data = Serial.readString(); 


//    lcd.println(data);
//    Serial.println(data);
    
    int index = (int) data.substring(24, data.length()-1).toInt();
    String color = data.substring(11, 17);
    int r = hexToInt(color.substring(0, 2));
    int g = hexToInt(color.substring(2, 4));
    int b = hexToInt(color.substring(4, 6));
//    Serial.print(index);
//    Serial.print(" ");
//    Serial.println(color);
//    Serial.print(r);
//    Serial.print(" ");
//    Serial.print(g);
//    Serial.print(" ");
//    Serial.println(b);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(index);
    lcd.setCursor(0,1);
    lcd.print(r);
    lcd.print(", ");
    lcd.print(g);
    lcd.print(", ");
    lcd.print(b);
    leds[index] = CRGB((byte) r, (byte) g, (byte) b); 
    FastLED.show(); 

    delay(100);
  }
}



int hexToInt(String str){
  int num = 0;
  for (int i = 0; i < str.length(); i++){
    num *= 16;
    if (str[i] < 'z' && str[i] >= 'a')
       num += str[i] - 'a' + 10;
    else
        num += str[i] - '0';
    }
    return num;
  }
