#include <Arduino.h>

#include "Communications/CommandExecution.h"
#include "ArmController/ArmController.h"
#include "GauntletController/GauntletController.h"

#include "ArmController/AngleCalculator.h"
#include "ArmController/ArmSensors.h"
#include "ArmController/ArmDriver.h"

void setup() {

    
    Serial.begin(9600);
    //init_communication();
    //init_arm();
    //init_gauntlet();

    //CURRENT SETUP: TEST ANGLE CALCULATOR
    //base angle: test between -180 and +180, actual between -90 and +90
    //forearm angle: between -180 and +180, actual between -120 and +50
    //wrist angle: between -180 and +180, actual between -90 and +90
    //assume that turntable is working properly

    float currentXY;
    float currentZ;

    float baseAngle;
    float foreAngle;
    float wristAngle;

    float startingBaseAngle;
    float startingForeAngle;

    delay(5000);

    //The base arm can go between -60 and +30 degrees
    for (startingBaseAngle = -60; startingBaseAngle <= 30; startingBaseAngle = startingBaseAngle + 10)
    {
        //The forearm should be able to go between -120 and 0 degrees
        for (startingForeAngle = -120; startingForeAngle <= 0; startingForeAngle = startingForeAngle + 10)
        {
            
            currentXY = calculate_xpos(0, startingBaseAngle, startingForeAngle);
            currentZ = calculate_zpos(startingBaseAngle, startingForeAngle);

            Serial.print("ROOT Base Angle: ");
            Serial.println(startingBaseAngle);
            Serial.print("ROOT Fore Angle: ");
            Serial.println(startingForeAngle);

            Serial.print("XY: ");
            Serial.println(currentXY);
            Serial.print("Z: ");
            Serial.println(currentZ);

            if (currentZ < 0)
            {
                Serial.println("                               Invalid Domain");
                continue;
            }

            if ( (startingBaseAngle + startingForeAngle) < -180 || (startingBaseAngle + startingForeAngle > 0) )
            {
                Serial.println("                               WRIST UNREACHABLE");
                continue;
            }

            baseAngle = calculate_arm_angle(currentXY, currentZ);
            foreAngle = calculate_forearm_angle(currentXY, currentZ);
            wristAngle = calculate_wrist_angle(baseAngle, foreAngle);

            if ( (baseAngle == UNREACHABLE_ERROR) || (foreAngle == UNREACHABLE_ERROR) || (wrist_angle == UNREACHABLE_ERROR))
            {
                Serial.println("                                UNREACHABLE");
            }
           
            Serial.print("Base Angle: ");
            Serial.println(baseAngle);

            Serial.print("Fore Angle: ");
            Serial.println(foreAngle);

            Serial.print("Wrist Angle: ");
            Serial.println(wristAngle);

            Serial.println();
            Serial.println();
            
        }
    }
    

}

void loop() 
{

}