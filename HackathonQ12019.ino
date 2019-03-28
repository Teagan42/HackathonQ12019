#include <FastLED.h>
#include <Arduino.h>
//#include "BeatDetector.h"
#include "LEDGrid.h"

#define MIC_PIN A2
#define HEART_LED_PIN 5
#define C_LED_PIN 6
#define V_LED_PIN 9
#define S_LED_PIN 10
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
//
//void preLoop();
//void postLoop();
//void onBeat(float amplitude);
//void offBeat();

//BeatDetector beatDetector(
//  MIC_PIN,
//  20,
//  preLoop,
//  postLoop,
//  onBeat,
//  offBeat
//);

#define WIDTH 8
#define HEIGHT 8
#define NUM_LEDS (WIDTH * HEIGHT)

CRGB heartLEDs[NUM_LEDS];
LEDGrid heartGrid(WIDTH, HEIGHT, true, heartLEDs);
CRGB cLEDs[NUM_LEDS];
LEDGrid cGrid(WIDTH, HEIGHT, true, cLEDs);
CRGB vLEDs[NUM_LEDS];
LEDGrid vGrid(WIDTH, HEIGHT, true, vLEDs);
CRGB sLEDs[NUM_LEDS];
LEDGrid sGrid(WIDTH, HEIGHT, true, sLEDs);
int hue = 0;

int heartMask[WIDTH][HEIGHT] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

int cMask[WIDTH][HEIGHT] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 0, 1, 1, 0},
  {0, 1, 1, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 0, 1, 1, 0},
  {0, 0, 0, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
int vMask[WIDTH][HEIGHT] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
int sMask[WIDTH][HEIGHT] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 1, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

void setup() {
  Serial.begin(115200);
  while (!Serial);

  FastLED.addLeds<LED_TYPE, HEART_LED_PIN, COLOR_ORDER>(heartLEDs, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, C_LED_PIN, COLOR_ORDER>(cLEDs, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, V_LED_PIN, COLOR_ORDER>(vLEDs, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, S_LED_PIN, COLOR_ORDER>(sLEDs, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(100);
  FastLED.clear();
 
  
//  beatDetector.setup();
}

void loop() {

  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      if (heartMask[x][y] != 0) {
          heartGrid.setPixel(x, y, CRGB(255, 0, 0));
      } else {
        heartGrid.setPixel(x, y, CRGB(0, 0, 0));
      }
      if (cMask[x][y] != 0) {
          cGrid.setPixel(x, y, CRGB(255, 0, 0));
      } else {
        cGrid.setPixel(x, y, CRGB(0, 0, 0));
      }
      if (vMask[x][y] != 0) {
          vGrid.setPixel(x, y, CRGB(255, 0, 0));
      } else {
        vGrid.setPixel(x, y, CRGB(0, 0, 0));
      }
      if (sMask[x][y] != 0) {
          sGrid.setPixel(x, y, CRGB(255, 0, 0));
      } else {
        sGrid.setPixel(x, y, CRGB(0, 0, 0));
      }
    }
    Serial.println();
  }
  FastLED.show();
  delay(10);
//  FastLED.clear();
//  leds[(currentIndex++) % NUM_LEDS] = CRGB(255,255,255);
//  FastLED.show();
//  delay(100);
//  // put your main code here, to run repeatedly:
//  beatDetector.loop();
}
//
//void preLoop() {}
//void postLoop() {}
//void onBeat(float amplitude) {
//  analogWrite(LED_PIN, (int)(255 - 3*amplitude));
//  Serial.print("I");
//}
//
//void offBeat() {
//  analogWrite(LED_PIN, 0);
//  Serial.print(".");
//}
