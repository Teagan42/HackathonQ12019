#ifndef __BEATDETECTOR_H__
#define __BEATDETECTOR_H__

#include <Arduino.h>
//#include <avdweb_AnalogReadFast.h>

// Our Global Sample Rate, 5000hz
#define SAMPLEPERIODUS 200

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

class BeatDetector {
  public:
    BeatDetector(
      int pinMic,
      float threshold,
      void (*preLoop)(),
      void (*postLoop)(),
      void (*onBeat)(float amplitude),
      void (*offBeat)()
    );

    void setup();

    void loop();

  private:
    float bassFilter(float sample);
    float envelopeFilter(float sample);
    float beatFilter(float sample);
    bool areOppositeSigns(float n1, float n2);

    int pinMic;
    float threshold;
    void (*preLoop)();
    void (*postLoop)();
    void (*onBeat)(float amplitude);
    void (*offBeat)();
};

#endif
