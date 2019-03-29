#define FASTLED_ALLOW_INTERRUPTS 0

#include <FastLED.h>
#include <Arduino.h>
//#include "BeatDetector.h"
#include "LEDGrid.h"
#include "IRCode.h"
#include <Servo.h>

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

int r = 255;
int g = 0;
int b = 0;
int brightness = 40;
int angle = 90;
boolean redraw = false;
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
  {0, 0, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

void setup() {
  //  irReceiver.setup();
  irReceiver.enableIRIn();
  servo.attach(SERVO_PIN);
  servo.write(angle);
  delay(100);
  servo.detach();

  drawLogo();

  //  beatDetector.setup();
}

void loop() {
  decode_results results;
  if (irReceiver.decode(&results)) {
    onReceive(results.value);
    irReceiver.resume();
  }

  if (writeServo) {
    moveServo();
    writeServo = false;
  }

  if (!redraw) {
    return;
  }
  redraw = false;
  drawLogo();
}

void drawLogo() {
  CRGB heartLEDs[NUM_LEDS];
  LEDGrid heartGrid(WIDTH, HEIGHT, true, heartLEDs);
  CRGB cLEDs[NUM_LEDS];
  LEDGrid cGrid(WIDTH, HEIGHT, true, cLEDs);
  CRGB vLEDs[NUM_LEDS];
  LEDGrid vGrid(WIDTH, HEIGHT, true, vLEDs);
  CRGB sLEDs[NUM_LEDS];
  LEDGrid sGrid(WIDTH, HEIGHT, true, sLEDs);
  CLEDController& heartCtl = FastLED.addLeds<LED_TYPE, HEART_LED_PIN, COLOR_ORDER>(heartLEDs, NUM_LEDS);
  CLEDController& cCtl = FastLED.addLeds<LED_TYPE, C_LED_PIN, COLOR_ORDER>(cLEDs, NUM_LEDS);
  CLEDController& vCtl = FastLED.addLeds<LED_TYPE, V_LED_PIN, COLOR_ORDER>(vLEDs, NUM_LEDS);
  CLEDController& sCtl = FastLED.addLeds<LED_TYPE, S_LED_PIN, COLOR_ORDER>(sLEDs, NUM_LEDS);
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
  FastLED.setBrightness(brightness);
  FastLED.clearData();
  heartCtl.clearLedData();
  heartCtl.clearLedData();
  heartCtl.clearLedData();
  heartCtl.clearLedData();
}

void moveServo() {
    servo.attach(SERVO_PIN);
    int current = servo.read();
    int increment = 5;
    if (current > angle) {
      increment = -5;
    }
    for(int i = current; i != angle; i += increment) {
      servo.write(i);
      delay(40);
    }
    servo.detach();
}

void onReceive(IRCode code) {
  switch (code) {
    case IRCode::left:
      angle -= 20;
      writeServo = true;
      break;
    case IRCode::right:
      angle += 20;
      writeServo = true;
      break;
    case IRCode::one:
      r -= 10;
      redraw = true;
      break;
    case IRCode::two:
      r = 255;
      g = 0;
      b = 0;
      redraw = true;
      break;
    case IRCode::three:
      r += 10;
      redraw = true;
      break;
    case IRCode::four:
      g -= 10;
      redraw = true;
      break;
    case IRCode::five:
      r = 0;
      g = 255;
      b = 0;
      redraw = true;
      break;
    case IRCode::six:
      g += 10;
      redraw = true;
      break;
    case IRCode::seven:
      b -= 10;
      redraw = true;
      break;
    case IRCode::eight:
      r = 0;
      g = 0;
      b = 255;
      redraw = true;
      break;
    case IRCode::nine:
      b += 10;
      redraw = true;
      break;
    case IRCode::vol_down:
      brightness -= 10;
      redraw = true;
      break;
    case IRCode::vol_up:
      brightness += 10;
      redraw = true;
      break;
  }

  r = constrain(r, 0, 255);
  g = constrain(g, 0, 255);
  b = constrain(b, 0, 255);
  angle = constrain(angle, 0, 180);
  brightness = constrain(brightness, 0, 255);
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
