#include <Arduino.h>

#include "StateControl/ReachRamp.h"
#include "AllPurposeInclude.h"

#define REACH_RAMP_TORQUE 0.4

void reach_ramp()
{
#if TESTING_ORDER_OF_EVENTS
    Serial.println("");
    Serial.println("");
    Serial.println("REACH_RAMP state entered!");
    Serial.println("______________________");
#endif
    

    request_arm_position__travel();

    //check collision
    if (robot_state() != REACH_RAMP) {
        return;
    }

    while (!ramp_reached()) {
        uint8_t response = follow_tape(REACH_RAMP_TORQUE);
        if (response == TAPE_NOT_FOUND) {
            backtrack_to_tape();
        }
        if (robot_state() != REACH_RAMP) {
            return;
        }
        Serial.println("going towards ramp"); // remove later
    }
    Serial.println("ramp reached");

    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(REACH_RAMP, ASCEND_RAMP);
    } else {
        switch_state(REACH_RAMP, MENU);
    }
}