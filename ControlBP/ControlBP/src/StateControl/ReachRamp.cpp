#include <Arduino.h>

#include "StateControl/ReachRamp.h"
#include "AllPurposeInclude.h"

#define REACH_RAMP_TORQUE 0.4

void reach_ramp()
{
    //int dev_or_comp = digitalRead(MASTER_SWITCH);
    /*
    Serial.print("IF this says 1, I was dev: ");
    Serial.println(dev_or_comp);

    Serial.println("");
    Serial.println("");
    Serial.println("REACH_RAMP state entered!");
    Serial.println("______________________");
    */

    request_arm_position__travel();

    //check collision
    if (robot_state() != REACH_RAMP) {
        return;
    }

    while (!U_turn_status()) {
        uint8_t response = follow_tape(REACH_RAMP_TORQUE);
        if (response == TAPE_NOT_FOUND) {
            backtrack_to_tape();
        }
        if (robot_state() != REACH_RAMP) {
            return;
        }
        /*
        Serial.print("IF this says 1, currently in dev: ");
        Serial.println(digitalRead(MASTER_SWITCH));
        */
        if (digitalRead(MASTER_SWITCH) == DEV) {
            // Serial.println("Breaking out of this rubbish loop");
            break;
        }
    }

    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(REACH_RAMP, ASCEND_RAMP);
    } else {
        switch_state(REACH_RAMP, MENU);
    }
}