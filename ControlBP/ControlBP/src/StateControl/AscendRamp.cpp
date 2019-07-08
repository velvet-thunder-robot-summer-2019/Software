#include "StateControl/AscendRamp.h"
#include "AllPurposeInclude.h"

void ascend_ramp()
{
    Serial.println("");
    Serial.println("");
    Serial.println("ASCEND_RAMP state entered!");
    Serial.println("______________________");

    request_arm_position__ascent();

    if (robot_state() != ASCEND_RAMP) {
        return;
    }

    //determine which side top of ramp tape would be at
    uint8_t tape_side;
    if (run_status.bot_identity == METHANOS) {
        tape_side = RIGHT;
    } else {
        tape_side = LEFT;
    }

    while (!branch_reached(tape_side)) {
        uint8_t response = follow_tape();
        if (response == TAPE_NOT_FOUND) {
            backtrack_to_tape();
        }
        if (robot_state() != ASCEND_RAMP) {
            return;
        }
    }

    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(ASCEND_RAMP, CALIBRATE);
    } else {
        switch_state(ASCEND_RAMP, MENU);
    }
}