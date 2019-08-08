#include "StateControl/AscendRamp.h"
#include "AllPurposeInclude.h"
#include "Locomotion/PID.h"

#define FLAT_GROUND_TIME 6000
#define STARTUP_TIME 300 

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
    uint32_t start_time = millis();
    while (millis() - start_time < STARTUP_TIME) {
        follow_tape(ASCEND_RAMP_PWM);
        if (robot_state() != ASCEND_RAMP) {
            return;
        }
    }
    
    int turn_direction = inevitable ? LEFT : RIGHT;
    set_kd_kp_for_reach_ramp();

    while (millis() - start_time < FLAT_GROUND_TIME) {
        follow_tape(REACH_RAMP_PWM);
        if (robot_state() != ASCEND_RAMP) {
            return;
        }
    }
    set_kd_kp_for_ramp();
    if (follow_tape_till_branch(ASCEND_RAMP, ASCEND_RAMP_PWM) == STATE_CHANGED) {
        return;
    }
    stop_motors();

    // ok so we've spotted the branch. We want to go left if Thanos, right if Methanos
    if (turn_onto_branch(turn_direction, ASCEND_RAMP) == STATE_CHANGED) {
        return;
    }
    location my_gauntlet = inevitable ? THANOS_GAUNTLET : METHANOS_GAUNTLET;
    location my_intersection = inevitable ? THANOS_INTERSECTION : METHANOS_INTERSECTION;
    update_position(my_gauntlet, my_intersection);

    set_kd_kp_for_post_nav();

    // ok so we should've turned onto the right branch, let's go into next state
    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(ASCEND_RAMP, FIND_POST);
    } else {
        switch_state(ASCEND_RAMP, MENU);
    }
}