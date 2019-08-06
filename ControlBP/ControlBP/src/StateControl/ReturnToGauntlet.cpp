#include "AllPurposeInclude.h"
#include "StateControl/ReturnToGauntlet.h"
#include "DecisionMaking/DecisionMaking.h"
#include "AllPurposeInclude.h"
#include "Debugging/Menu.h"

#define GO_INTO_GAUNTLET_TIME 1000 // 2 seconds

int return_from_lower_path();
int return_from_upper_path();

void return_to_gauntlet()
{
#if TESTING_ORDER_OF_EVENTS
    Serial.println("RETURN_TO_GAUNTLET state entered!");
    Serial.println("______________________");
    Serial.println("starting state: ");
    Serial.println(robot_state());
#endif
    bool inevitable = run_status.bot_identity == THANOS;

    location my_fourth_post_lower = inevitable ? POST_1 : POST_4;
    location my_second_post_upper = inevitable ? POST_6 : POST_5;

    if (run_status.bot_position.last_location == my_fourth_post_lower) {
        return_from_lower_path();
    } 
    if (run_status.bot_position.last_location == my_second_post_upper) {
        return_from_upper_path();
    }


    // hardcode in way in
    int start_time = millis();
    while (millis() - start_time < GO_INTO_GAUNTLET_TIME) {
        follow_tape(FLAT_GROUND_APPROACHING_STOP_PWM);
    }
    stop_motors();

    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(RETURN_TO_GAUNTLET, FIT_TO_GAUNTLET);
    } else {
        switch_state(RETURN_TO_GAUNTLET, MENU);
    }
}

int return_from_lower_path()
{
    // this is the case where we've hit up 3 posts and life is chill. 
    bool inevitable = run_status.bot_identity == THANOS;
    location my_gauntlet = inevitable ? THANOS_GAUNTLET : METHANOS_GAUNTLET;
    location my_intersection = inevitable ? THANOS_INTERSECTION : METHANOS_INTERSECTION;

    if (robot_state() != RETURN_TO_GAUNTLET) {
#if TESTING_ORDER_OF_EVENTS
    Serial.println("status changed");
    Serial.print("status is: ");
    Serial.println(robot_state());
#endif
        return STATE_CHANGED;
    }
    if (return_to_intersection(RETURN_TO_GAUNTLET) == STATE_CHANGED) {
        return STATE_CHANGED;
    }

    int direction = inevitable ? RIGHT : LEFT;
    if (turn_onto_branch(direction, RETURN_TO_GAUNTLET) == STATE_CHANGED)
    {
        return STATE_CHANGED;
    }
    update_position(my_intersection, my_gauntlet);

#if DEBUG_SCREEN_DELAYS
    display_string("inter-gaunt");
    delay(2000);
#endif

    if (follow_tape_till_branch(RETURN_TO_GAUNTLET, FLAT_GROUND_TAPE_FOLLOWING_PWM) == STATE_CHANGED) {
        return STATE_CHANGED;
    }
    
    direction = inevitable ? LEFT : RIGHT;
    if (turn_into_gauntlet(direction, RETURN_TO_GAUNTLET)) {
        return STATE_CHANGED;
    }
    return SUCCESS;
}

int return_from_upper_path()
{
    // this is the case where we've hit up 3 posts and life is chill. 
    bool inevitable = run_status.bot_identity == THANOS;
    location my_gauntlet = inevitable ? THANOS_GAUNTLET : METHANOS_GAUNTLET;
    location my_intersection = inevitable ? THANOS_INTERSECTION : METHANOS_INTERSECTION;

    if (robot_state() != RETURN_TO_GAUNTLET) {
#if TESTING_ORDER_OF_EVENTS
    Serial.println("status changed");
    Serial.print("status is: ");
    Serial.println(robot_state());
#endif
        return STATE_CHANGED;
    }
    return_to_intersection(RETURN_TO_GAUNTLET);
    int direction = inevitable ? LEFT : RIGHT;
    if (turn_onto_branch(direction, RETURN_TO_GAUNTLET) == STATE_CHANGED)
    {
        return STATE_CHANGED;
    }
    update_position(my_intersection, my_gauntlet);

    if (follow_tape_till_branch(RETURN_TO_GAUNTLET, FLAT_GROUND_TAPE_FOLLOWING_PWM) == STATE_CHANGED) {
        return STATE_CHANGED;
    }
    
    direction = inevitable ? LEFT : RIGHT;
    if (turn_onto_branch(direction, RETURN_TO_GAUNTLET)) {
        return STATE_CHANGED;
    }
    return SUCCESS;
}
