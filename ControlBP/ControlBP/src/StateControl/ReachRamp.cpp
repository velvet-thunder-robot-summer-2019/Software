#include <Arduino.h>

#include "StateControl/ReachRamp.h"
#include "AllPurposeInclude.h"


void reach_ramp()
{
    Serial.println("REACH_RAMP state entered!");
    Serial.println("______________________");

    request_arm_position__travel();

    //check collision
    if (bot_state != REACH_RAMP) {
        return;
    }

    while (!request_U_turn_status()) {
        uint8_t response = follow_tape();
        if (response == TAPE_NOT_FOUND) {
            backtrack_to_tape();
        }
        if (bot_state != REACH_RAMP) {
            return;
        }
    }

    bot_previous_state = REACH_RAMP;
    if (digitalRead(MASTER_SWITCH) == COMP) {
        bot_state = ASCEND_RAMP;
    } else {
        bot_state = MENU;
    }
}