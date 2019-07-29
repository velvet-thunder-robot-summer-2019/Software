#include <Arduino.h>

#include "Communications/CommandExecution.h"
#include "ArmController/ArmController.h"
#include "GauntletController/GauntletController.h"

#include "ArmController/AngleCalculator.h"
#include "ArmController/ArmSensors.h"
#include "ArmController/ArmDriver.h"

uint16_t delayTime, baseTime;

int16_t xyTemp, zTemp;
int16_t baseAngle, foreAngle, wristAngle;

Servo testServo;

void setup() {

    //Initiialize ALL the peripherals
    Serial.begin(9600);
    //init_communication();
    init_arm();
    //init_gauntlet();

    //Test move the arm

    delay(10000);


    /*
    delay(5000);

    Serial.println("Init Successful");

    base_arm_angle = 30.0;
    forearm_angle = 30.0;

    move_whole_arm_position(base_arm_angle, forearm_angle, 0, 0);

    Serial.println(" -------> MAINTAIN POS <-------- ");
    */

    //pwm_start(CLAW_SERVO_PIN, 10000, 200, 22, 1);

    //open_claw();
    //pwm_start(WRIST_SERVO_PIN, 10000, 200, 22 - ( (float) 45 / 11.25), 1);

    //calibration: 11.25 degrees per unit

    // (22 - x) * 11.25

    //open_claw();


}

void loop() {

    pwm_start(BASE_ARM_CW_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, PWM_PERIOD * 0.50, 0);
    
    // base_arm_angle = 60.0;
    // forearm_angle = 60.0;
    // wrist_angle = 45.0;

    // Serial.println("Reaching Position...");

    // move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, 0);
    
    // delay(200);

    // Serial.println("MAINTAIN");

    // while(1)
    // {
    //     maintain_current_arm_position();
    // }
    

}