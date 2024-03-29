#include "StateControl/AscendRamp.h"
#include "AllPurposeInclude.h"

#define FLAT_GROUND_TIME 5000

void ascend_ramp()
{
#if TESTING_ORDER_OF_EVENTS
    Serial.println("");
    Serial.println("");
    Serial.println("ASCEND_RAMP state entered!");    
    Serial.println("______________________");
#endif

    if (robot_state() != ASCEND_RAMP) {
        return;
    }
    int inevitable = run_status.bot_identity == THANOS;
    
    int turn_direction = inevitable ? LEFT : RIGHT;

    uint32_t start_time = millis();
    while (millis() - start_time < FLAT_GROUND_TIME) {
        follow_tape(REACH_RAMP_PWM);
        if (robot_state() != ASCEND_RAMP) {
            return;
        }
    }
    
    if (follow_tape_till_branch(ASCEND_RAMP, ASCEND_RAMP_PWM) == STATE_CHANGED) {
        return;
    }
    // stop_motors();
    // delay(3000);
    // ok so we've spotted the branch. We want to go left if Thanos, right if Methanos

    if (turn_onto_branch(turn_direction, ASCEND_RAMP) == STATE_CHANGED) {
        return;
    }
    location my_gauntlet = inevitable ? THANOS_GAUNTLET : METHANOS_GAUNTLET;
    location my_intersection = inevitable ? THANOS_INTERSECTION : METHANOS_INTERSECTION;
    update_position(my_gauntlet, my_intersection);

    // ok so we should've turned onto the right branch, let's go into next state
    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(ASCEND_RAMP, FIND_POST);
    } else {
        switch_state(ASCEND_RAMP, MENU);
    }
}