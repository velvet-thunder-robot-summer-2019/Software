#include <Arduino.h>

#include "CommandExecution.h"
#include "ArmController/ArmController.h"
#include "GauntletController/GauntletController.h"

void setup() {

    Serial.begin(9600);
    init_communication();
    init_arm();
    init_gauntlet();

}

void loop() {

  execute_command(); // if that is, there's a command to be executed
  
}