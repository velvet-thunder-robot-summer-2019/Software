#include <Arduino.h>

#include "StateControl/AscendRamp.h"
#include "AllPurposeInclude.h"
#include "Locomotion/TapeSensor.h"

void ascend_ramp()
{
    Serial.println("ASCEND_RAMP state entered!");
    Serial.println("______________________");

    request_arm_position__ascent();

    if (bot_state != REACH_RAMP) {
        return;
    }

    //determine which side top of ramp tape would be at
    uint8_t tape_side;
    if (bot_identity == METHANOS) {
        tape_side = RIGHT;
    } else {
        tape_side = LEFT;
    }

    while (!branch_reached(tape_side)) {
        uint8_t response = follow_tape();
        if (response == TAPE_NOT_FOUND) {
            backtrack_to_tape();
        }
        if (bot_state != REACH_RAMP) {
            return;
        }
    }

    bot_previous_state = ASCEND_RAMP;
    if (digitalRead(MASTER_SWITCH) == COMP) {
        bot_state = CALIBRATE;
    } else {
        bot_state = MENU;
    }
}