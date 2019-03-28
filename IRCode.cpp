#include "IRCode.h"

IRReceiver::IRReceiver(int pin, void (*onReceive)(IRCode code)) {
  this->pin = pin;
  this->onReceive = onReceive;
}

void IRReceiver::setup() {
  Serial.println("Enabling IR Input");
  IRrecv irrecv(this->pin);

  this->receiver = &irrecv;
  
  this->receiver->enableIRIn();
}
void IRReceiver::loop() {
  if (this->receiver->isIdle()) {
    return;
  }
  Serial.print("Checking IR: ");
  decode_results results;
  if (this->receiver->decode(&results)) {
    Serial.println(" RECEIVED");
    this->onReceive(results.value);
  } else {
    Serial.println("N/A");
  }
  this->receiver->resume();
}
