#include <FastLED.h> // FastLED library

#define NUM_LEDS 144 // number of leds
#define DATA_PIN 6 // data pin for output (middle pin of WS2812B)
#define WAIT 15 // wait time between frames of animation

CRGB leds[NUM_LEDS]; // array of leds (144 leds) for FastLED library to use

unsigned long colorArray[6] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF}; // array of colors
// array of colors GRB for some reason maybe the wrong chip selection

void setup() {
  FastLED.addLeds<WS2812B,DATA_PIN>(leds, NUM_LEDS); // add leds to FastLED library
} // end setup

void loop() {
  for (int i = 1; i < NUM_LEDS-1; i++) { // loop through leds
    leds[i] = colorArray[i%6]; // set leds to color array
    FastLED.show(); // show leds
    delay(WAIT); // wait
    FastLED.clear(); // clear leds
    FastLED.show(); // show leds
  } //end for
    for (int i = NUM_LEDS; i > 1; i--) {
    leds[i] = colorArray[i%6]; // set leds to color array (reverse)
    FastLED.show(); // show leds
    delay(WAIT); // wait
    FastLED.clear(); // clear leds
    FastLED.show(); // show leds
  } //end for
} // end loop
