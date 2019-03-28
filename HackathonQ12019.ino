#include <FastLED.h>
#include <Arduino.h>
//#include "BeatDetector.h"
#include "LEDGrid.h"
#include "IRCode.h"

#define MIC_PIN A2
#define HEART_LED_PIN 5
#define C_LED_PIN 6
#define V_LED_PIN 9
#define S_LED_PIN 10
#define IR_PIN 11

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


//void onReceive(IRCode code);
//IRReceiver irReceiver(IR_PIN, onReceive);
//IRrecv irReceiver(IR_PIN);
//decode_results results;

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
int r = 100;
int g = 100;
int b = 100;
boolean redraw = true;

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

  //  irReceiver.setup();
//  irReceiver.enableIRIn();

  Serial.println("Initializing LEDs");

    FastLED.addLeds<LED_TYPE, HEART_LED_PIN, COLOR_ORDER>(heartLEDs, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, C_LED_PIN, COLOR_ORDER>(cLEDs, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, V_LED_PIN, COLOR_ORDER>(vLEDs, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, S_LED_PIN, COLOR_ORDER>(sLEDs, NUM_LEDS);
  FastLED.setBrightness(100);
  FastLED.clear();

  //  beatDetector.setup();
}

void loop() {
//  if (irReceiver.decode(&results)) {
//    Serial.println(results.value, HEX);
//    onReceive(results.value);
//    irReceiver.resume();
//    redraw = true;
//  }

  if (!redraw) {
    return;
  }
  redraw = false;
  drawLogo();
}

void drawLogo() {
  Serial.println("Drawing Logo");
  CRGB activeColor(r, g, b);
  CRGB inactiveColor(0, 0, 0);
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
            if (heartMask[x][y] != 0) {
                heartGrid.setPixel(x, y, activeColor);
            } else {
              heartGrid.setPixel(x, y, inactiveColor);
            }
      if (cMask[x][y] != 0) {
        cGrid.setPixel(x, y, activeColor);
      } else {
        cGrid.setPixel(x, y, inactiveColor);
      }
      if (vMask[x][y] != 0) {
        vGrid.setPixel(x, y, activeColor);
      } else {
        vGrid.setPixel(x, y, inactiveColor);
      }
      if (sMask[x][y] != 0) {
        sGrid.setPixel(x, y, activeColor);
      } else {
        sGrid.setPixel(x, y, inactiveColor);
      }
    }
  }
  FastLED.show();
}

void onReceive(IRCode code) {
  switch (code) {
    case IRCode::up:
      r = (r + 10) % 255;
      break;
    case IRCode::down:
      r = (r - 10) % 255;
      break;
    case IRCode::left:
      g = (g + 10) % 255;
      break;
    case IRCode::right:
      g = (g - 10) % 255;
      break;
    case IRCode::one:
      b = (b - 10) % 255;
      break;
    case IRCode::three:
      b = (b + 10) % 255;
      break;
  }
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
