#include "BeatDetector.h"

BeatDetector::BeatDetector(int pinMic, float threshhold, void (*preLoop)(), void (*postLoop)(), void (*onBeat)(float amplitude), void (*offBeat)()) {
  this->pinMic = pinMic;
  this->threshold = threshold;
  this->preLoop = preLoop;
  this->postLoop = postLoop;
  this->onBeat = onBeat;
  this->offBeat = offBeat;
}

void BeatDetector::setup() {
  pinMode(this->pinMic, INPUT);
}

void BeatDetector::loop() {
  unsigned long time = micros(); // Used to track rate
  float sample, value, envelope, beat, lastBeat;
  unsigned char i;

  for (i = 0;; ++i) {
    // Read ADC and center so +-512
    sample = (float)analogReadFast(this->pinMic) - 503.f;

    // Filter only bass component
    value = bassFilter(sample);

    // Take signal amplitude and filter
    if (value < 0)value = -value;
    envelope = envelopeFilter(value);

    // Every 200 samples (25hz) filter the envelope
    if (i == 200) {
      this->preLoop();
      lastBeat = beat;
      // Filter out repeating bass sounds 100 - 180bpm
      beat = beatFilter(envelope);

      // If we are above threshold, light up LED
      if (beat > this->threshold) this->onBeat(beat);
      else this->offBeat();

      //Reset sample counter
      i = 0;
      this->postLoop();
    }

    // Consume excess clock cycles, to keep at 5000 hz
    for (unsigned long up = time + SAMPLEPERIODUS; time > 20 && time < up; time = micros());
  }
}

// 20 - 200hz Single Pole Bandpass IIR Filter
float BeatDetector::bassFilter(float sample) {
  static float xv[3] = {0, 0, 0}, yv[3] = {0, 0, 0};
  xv[0] = xv[1]; xv[1] = xv[2];
  xv[2] = sample / 9.1f;
  yv[0] = yv[1]; yv[1] = yv[2];
  yv[2] = (xv[2] - xv[0])
          + (-0.7960060012f * yv[0]) + (1.7903124146f * yv[1]);
  return yv[2];
}

// 10hz Single Pole Lowpass IIR Filter
float BeatDetector::envelopeFilter(float sample) { //10hz low pass
  static float xv[2] = {0, 0}, yv[2] = {0, 0};
  xv[0] = xv[1];
  xv[1] = sample / 160.f;
  yv[0] = yv[1];
  yv[1] = (xv[0] + xv[1]) + (0.9875119299f * yv[0]);
  return yv[1];
}

// 1.7 - 3.0hz Single Pole Bandpass IIR Filter
float BeatDetector::beatFilter(float sample) {
  static float xv[3] = {0, 0, 0}, yv[3] = {0, 0, 0};
  xv[0] = xv[1]; xv[1] = xv[2];
  xv[2] = sample / 7.015f;
  yv[0] = yv[1]; yv[1] = yv[2];
  yv[2] = (xv[2] - xv[0])
          + (-0.7169861741f * yv[0]) + (1.4453653501f * yv[1]);
  return yv[2];
}

bool BeatDetector::areOppositeSigns(float n1, float n2) {
  return (pow(n1, n2) < 0);
}
