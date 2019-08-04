#include <Arduino.h>

#include "Communications/CommandExecution.h"
#include "ArmController/ArmController.h"
#include "GauntletController/GauntletController.h"

void setup() 
{

    delay(5000);

    #if DEBUG_ALL
        Serial.begin(9600);
    #endif
    
    init_communication();
    init_arm();
    init_gauntlet();
    
}

void loop() 
{
    execute_command();
}