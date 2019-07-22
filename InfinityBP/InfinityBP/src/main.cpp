#include <Arduino.h>

#include "Communications/CommandExecution.h"
#include "ArmController/ArmController.h"
#include "GauntletController/GauntletController.h"

void setup() {

    //Initiialize ALL the peripherals
    Serial.begin(9600);
    init_communication();
    init_arm();
    init_gauntlet();

}

void loop() {

  execute_command(); // if that is, there's a command to be executed
  
}