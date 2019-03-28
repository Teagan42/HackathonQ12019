
#include <IRremote.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

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

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
}


void loop() {
  
  if (irrecv.decode(&results)) {
    switch(results.value) {
      case IRCode::up : Serial.println("Up");
      case IRCode::down : Serial.println("Down");
      case IRCode::left : Serial.println("Left");
      case IRCode::Right : Serial.println("Right");
    }
    irrecv.resume();
  }
  delay(100);    
}

