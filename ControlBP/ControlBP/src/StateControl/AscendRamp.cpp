#include "StateControl/AscendRamp.h"
#include "AllPurposeInclude.h"

#define ASCEND_RAMP_TORQUE 10

void ascend_ramp()
{
    // Serial.println("");
    // Serial.println("");
    // Serial.println("ASCEND_RAMP state entered!");    
    // Serial.println("______________________");

    request_arm_position__ascent();

    if (robot_state() != ASCEND_RAMP) {
        return;
    }

    //determine which side top of ramp tape would be at
    // uint8_t tape_side;
    // if (run_status.bot_identity == METHANOS) {
    //     tape_side = RIGHT;
    // } else {
    //     tape_side = LEFT;
    // }

    while (!branch_reached_front()) {
        uint8_t response = follow_tape(ASCEND_RAMP_TORQUE);
        if (response == TAPE_NOT_FOUND) {
            backtrack_to_tape();
        }
        if (robot_state() != ASCEND_RAMP) {
            return;
        }
    }
    // ok so we've spotted the branch. We want to go left if Thanos, right if Methanos
    
    int branch_to_follow = run_status.bot_identity == THANOS ? LEFT : RIGHT;

    if (turn_onto_branch(branch_to_follow) == STATE_CHANGED) {
        return;
    }

    // ok so we should've turned onto the right branch, let's go into next state
    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(ASCEND_RAMP, FIND_POST);
    } else {
        switch_state(ASCEND_RAMP, MENU);
    }
}