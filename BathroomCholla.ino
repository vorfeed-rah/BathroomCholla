#include <SPI.h>
#include "FastLED.h"

//you used a duemilanove for this project!

#define NUM_LEDS_PER_STRIP 2

//#pragma GCC optimize ("-O2") //this tells the Arduino compiler to prioritize execution speed over program size

unsigned long fadeTimer = 0;
unsigned long updateTimer = 0;

#define FADE_LOW 16000
#define FADE_HIGH 35000 //randomly pick a time to change palettes
#define UPDATE_LOW 50
#define UPDATE_HIGH 100 //these randomly pick an update time for the lights

#define GREENPAL 1
#define GREENYELLOWPAL 2
#define HARVESTPAL 3

int cur_pal = GREENPAL;
int update_interval = 100; // 50 timer to update colors
int fade_interval = 16000; // 16 seconds between palettes

CRGB leds[NUM_LEDS_PER_STRIP];

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;

//our custom color palette. should have 16 different "web colors" listed
const TProgmemPalette16 greenPalette PROGMEM =
{
    CRGB::OliveDrab,
    CRGB::OliveDrab,
    CRGB::OliveDrab,
    CRGB::ForestGreen,
    CRGB::ForestGreen,
    CRGB::MediumSeaGreen,
    CRGB::MediumSpringGreen,
    CRGB::MediumSpringGreen,
    CRGB::MediumSeaGreen,
    CRGB::ForestGreen,
    CRGB::ForestGreen,
    CRGB::DarkGreen,
    CRGB::Green,
    CRGB::DarkGreen,
    CRGB::DarkOliveGreen,
    CRGB::DarkOliveGreen,
};
//our custom color palette. should have 16 different "web colors" listed
const TProgmemPalette16 greenYellowPalette PROGMEM =
{
    CRGB::OliveDrab,
    CRGB::OliveDrab,
    CRGB::OliveDrab,
    CRGB::ForestGreen,
    CRGB::ForestGreen,
    CRGB::GreenYellow,
    CRGB::GreenYellow,
    CRGB::Gold,
    CRGB::GreenYellow,
    CRGB::ForestGreen,
    CRGB::ForestGreen,
    CRGB::GreenYellow,
    CRGB::PaleGreen,
    CRGB::Green,
    CRGB::ForestGreen,
    CRGB::ForestGreen,
};
//our custom color palette. should have 16 different "web colors" listed
const TProgmemPalette16 harvestPalette PROGMEM =
{
    CRGB::OliveDrab,
    CRGB::OliveDrab,
    CRGB::DarkGoldenrod,
    CRGB::DarkGoldenrod,
    CRGB::Chocolate,
    CRGB::Coral,
    CRGB::Chocolate,
    CRGB::DarkGoldenrod,
    CRGB::Gold,
    CRGB::Goldenrod,
    CRGB::GreenYellow,
    CRGB::Green,
    CRGB::Green,
    CRGB::ForestGreen,
    CRGB::ForestGreen,
    CRGB::DarkOliveGreen,
};

void setup()
{

  Serial.begin(115200);

  FastLED.addLeds<WS2811, 6>(leds, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2811, 7>(leds, NUM_LEDS_PER_STRIP);
  
  currentPalette = harvestPalette;
  targetPalette = harvestPalette;
  cur_pal = HARVESTPAL;
  currentBlending = LINEARBLEND;

}

long previous = 0;
long current = 0;
long t1;
int k = 0;

void loop()
{
  previous = current;
  current = micros();
  t1 = current - previous;
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1;
    
  ChangePalettePeriodically();
  uint8_t maxChanges = 24; 
  nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);

   if(millis() > updateTimer ) //updates the colors every X milliseconds
  {
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; 
    
    //this fills all the LEDs with our my_palette colors
    FillLEDsFromPaletteColors( startIndex); //picks new colors as below
    update_interval = random(UPDATE_LOW, UPDATE_HIGH);
    updateTimer = millis() + update_interval;    
  }
    
  FastLED.show(); 
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 100; //sets brightness for the strip
  
    for( int i = 0; i < NUM_LEDS_PER_STRIP; i++) { //fills each individual pixel with the next color, for a "swim" effect
       leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
       colorIndex += 3;
    }
  
}

void ChangePalettePeriodically()
{

 if(millis() > fadeTimer ) //updates the colors every X milliseconds
  {
    
    int next_pal = random(4); //picks a random palette to change to
    
    if (next_pal == GREENPAL) {
      Serial.println("moving to greenYellowPal");
      targetPalette = greenYellowPalette;
      cur_pal = GREENYELLOWPAL;
      fade_interval = random(FADE_LOW, FADE_HIGH);
      fadeTimer = millis() + fade_interval;
      return;
    }
    
    if (next_pal == GREENYELLOWPAL) {
       Serial.println("moving to harvestPal");
      targetPalette = harvestPalette; 
      cur_pal = HARVESTPAL;
      fade_interval = random(FADE_LOW, FADE_HIGH);
      fadeTimer = millis() + fade_interval;
      return;
    }
    
    if (next_pal == HARVESTPAL) {
       Serial.println("moving to greenPal");
      targetPalette = greenPalette;
      cur_pal = GREENPAL;
       fade_interval = random(FADE_LOW, FADE_HIGH);
      fadeTimer = millis() + fade_interval;
      return;
    }
    
      fade_interval = random(FADE_LOW, FADE_HIGH);
      fadeTimer = millis() + fade_interval;
  }
} 
