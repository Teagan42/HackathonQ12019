#include <FastLED.h>
#include <Arduino.h>
//#include "BeatDetector.h"
#include "LEDGrid.h"
#include "IRCode.h"

#define MIC_PIN A5
#define SERVO_PIN A4
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
IRrecv irReceiver(IR_PIN);
Servo servo;

#define WIDTH 8
#define HEIGHT 8
#define NUM_LEDS (WIDTH * HEIGHT)

int r = 100;
int g = 100;
int b = 100;
int angle = 90;
boolean redraw = true;
boolean writeServo = false;

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
  irReceiver.enableIRIn();
  servo.attach(SERVO_PIN);
  servo.write(90);

  //  beatDetector.setup();
}

void loop() {
  decode_results results;
  if (irReceiver.decode(&results)) {
    Serial.println(results.value, HEX);
    onReceive(results.value);
    irReceiver.resume();
  }

  if (writeServo) {
    servo.write(angle);
    writeServo = false;
  }

  if (!redraw) {
    return;
  }
  redraw = false;
  drawLogo();
}

void drawLogo() {
  Serial.println("Drawing Logo");
  CRGB heartLEDs[NUM_LEDS];
  LEDGrid heartGrid(WIDTH, HEIGHT, true, heartLEDs);
  CRGB cLEDs[NUM_LEDS];
  LEDGrid cGrid(WIDTH, HEIGHT, true, cLEDs);
  CRGB vLEDs[NUM_LEDS];
  LEDGrid vGrid(WIDTH, HEIGHT, true, vLEDs);
  CRGB sLEDs[NUM_LEDS];
  LEDGrid sGrid(WIDTH, HEIGHT, true, sLEDs);
  FastLED.addLeds<LED_TYPE, HEART_LED_PIN, COLOR_ORDER>(heartLEDs, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, C_LED_PIN, COLOR_ORDER>(cLEDs, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, V_LED_PIN, COLOR_ORDER>(vLEDs, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, S_LED_PIN, COLOR_ORDER>(sLEDs, NUM_LEDS);
  FastLED.clear();

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
  FastLED.setBrightness(100);
  FastLED.clearData();
}

void onReceive(IRCode code) {
  switch (code) {
    case IRCode::up:
      r += 10;
      redraw = true;
      break;
    case IRCode::down:
      r -= 10;
      redraw = true;
      break;
    case IRCode::left:
      angle -= 5;
      writeServo = true;
      break;
    case IRCode::right:
      angle += 5;
      writeServo = true;
      break;
    case IRCode::one:
      b -= 10;
      redraw = true;
      break;
    case IRCode::three:
      b += 10;
      redraw = true;
      break;
  }

  r = constrain(r, 0, 255);
  g = constrain(g, 0, 255);
  b = constrain(b, 0, 255);
  angle = constrain(angle, 0, 180);
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
