#include "IRCode.h"

IRReceiver::IRReceiver(int receiverPin, int servoPin) {
  this->receiverPin = receiverPin;
  this->servoPin = servoPin;
}

void IRReceiver::setup() {
  Serial.println("Enabling IR Input");
  IRrecv irrecv(this->receiverPin);
  Servo serv;

  this->receiver = &irrecv;
  this->servo = &serv;
  
  this->servo->attach(servoPin);
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
    //this->onReceive(results.value);
    this->servo->write(90);
  } else {
    Serial.println("N/A");
  }
  this->receiver->resume();
}
