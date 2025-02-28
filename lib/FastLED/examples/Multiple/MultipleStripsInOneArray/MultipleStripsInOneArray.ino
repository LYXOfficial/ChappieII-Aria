/// @file    MultipleStripsInOneArray.ino
/// @brief   Demonstrates how to use multiple LED strips, each with their own data in one shared array
/// @example MultipleStripsInOneArray.ino

// MultipleStripsInOneArray - see https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples for more info on
// using multiple controllers.  In this example, we're going to set up four NEOPIXEL strips on three
// different pins, each strip will be referring to a different part of the single led array

#include <FastLED.h>

#define NUM_STRIPS 3
#define NUM_LEDS_PER_STRIP 60
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

// For mirroring strips, all the "special" stuff happens just in setup.  We
// just addLeds multiple times, once for each strip
void setup() {
  // tell FastLED there's 60 NEOPIXEL leds on pin 2, starting at index 0 in the led array
  FastLED.addLeds<NEOPIXEL, 2>(leds, 0, NUM_LEDS_PER_STRIP);

  // tell FastLED there's 60 NEOPIXEL leds on pin 3, starting at index 60 in the led array
  FastLED.addLeds<NEOPIXEL, 3>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);

  // tell FastLED there's 60 NEOPIXEL leds on pin 4, starting at index 120 in the led array
  FastLED.addLeds<NEOPIXEL, 4>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);

}

void loop() {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    FastLED.show();
    leds[i] = CRGB::Black;
    delay(100);
  }
}
