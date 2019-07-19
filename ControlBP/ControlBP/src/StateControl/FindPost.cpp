#include "AllPurposeInclude.h"
#include "DecisionMaking/DecisionMaking.h"
#include "StateControl/FindPost.h"

int debug_iterations = 0;

void find_post()
{
    // Serial.println("");
    // Serial.println("");
    // Serial.println("FIND_POST state entered!");
    // Serial.println("______________________");
    bool I_am_inevitable = run_status.bot_identity == THANOS;
    location my_gauntlet = I_am_inevitable ? THANOS_GAUNTLET : METHANOS_GAUNTLET;
    location my_intersection = I_am_inevitable ? THANOS_INTERSECTION : METHANOS_INTERSECTION;

    if (run_status.bot_position.last_location == my_gauntlet && run_status.bot_position.next_location == my_intersection) {
        // location next_location = I_am_inevitable ? POST_4 : POST_1;
        // int next_turn_direction = I_am_inevitable ? LEFT : RIGHT;

        while (!branch_reached_front()) {
            follow_tape(FLAT_GROUND_TAPE_FOLLOWING_PWM);
            if (run_status.bot_state != FIND_POST) {
                return;
            }
        }
    }

    if (robot_state() != FIND_POST) {
        return;
    }

  
    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(FIND_POST, GET_INFINITY_STONE);
    } else {
        switch_state(FIND_POST, MENU);
    }
}
