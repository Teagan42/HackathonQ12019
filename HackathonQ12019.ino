#include "BeatDetector.h"

#define MIC_PIN A2
#define LED_PIN A3

void preLoop();
void postLoop();
void onBeat(float amplitude);
void offBeat();

BeatDetector beatDetector(
  MIC_PIN,
  0.5,
  preLoop,
  postLoop,
  onBeat,
  offBeat
);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);
  beatDetector.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  beatDetector.loop();
}

void preLoop() {}
void postLoop() {}
void onBeat(float amplitude) {
  analogWrite(LED_PIN, (int)(255 - 3*amplitude));
  Serial.print("I");
}

void offBeat() {
  analogWrite(LED_PIN, 0);
  Serial.print(".");
}
