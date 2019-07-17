#include <Arduino.h>

#include "CommandExecution.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  init_communication();
}

void loop() {
  // put your main code here, to run repeatedly:
  execute_command(); // if that is, there's a command to be executed
}