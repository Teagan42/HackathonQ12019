#ifndef __IR_CODE_H__
#define __IR_CODE_H__

#include <IRremote.h>
#include <Servo.h>

enum IRCode {
  up = 16621663,
  down = 16625743,
  left = 16584943,
  right = 16601263,
  one = 16582903,
  two = 16615543,
  three = 16599223,
  four = 16591063,
  five = 16623703,
  six = 16607383,
  seven = 16586983,
  eight = 16619623,
  nine = 16603303,
  zero = 16593103,
  enter = 16617583,
  invalid = 4294967295
};

class IRReceiver {
  public:
    IRReceiver(int receiverPin, int servoPin);

    void setup();
    void loop();
  private:
    int receiverPin;
    int servoPin;
    IRrecv* receiver;
    Servo* servo;
};

#endif
