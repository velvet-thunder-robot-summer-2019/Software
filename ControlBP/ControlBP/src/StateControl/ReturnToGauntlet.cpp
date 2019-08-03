#include "AllPurposeInclude.h"
#include "StateControl/ReturnToGauntlet.h"
#include "DecisionMaking/DecisionMaking.h"
#include "AllPurposeInclude.h"

#define GO_INTO_GAUNTLET_TIME 2000 // 2 seconds

int return_from_lower_post_and_intersect();
int return_from_upper_post_and_intersect();

void return_to_gauntlet()
{
#if TESTING_ORDER_OF_EVENTS
    Serial.println("RETURN_TO_GAUNTLET state entered!");
    Serial.println("______________________");
    Serial.println("starting state: ");
    Serial.println(robot_state());
#endif


    // hardcode in way in
    int start_time = millis();
    while (millis() - start_time < GO_INTO_GAUNTLET_TIME) {
        follow_tape(FLAT_GROUND_APPROACHING_STOP_PWM);
    }

    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(RETURN_TO_GAUNTLET, FIT_TO_GAUNTLET);
    } else {
        switch_state(RETURN_TO_GAUNTLET, MENU);
    }
}

int return_from_lower_post_and_intersect()
{
    // this is the case where we've hit up 3 posts and life is chill. 
    bool inevitable = run_status.bot_identity == THANOS;
    location my_gauntlet = inevitable ? THANOS_GAUNTLET : METHANOS_GAUNTLET;
    location my_intersection = inevitable ? THANOS_INTERSECTION : METHANOS_INTERSECTION;
    location my_first_post = inevitable ? POST_4 : POST_1;
    location my_second_post = inevitable ? POST_3 : POST_2;
    location my_third_post = inevitable ? POST_2 : POST_3;
    location my_fourth_post = inevitable ? POST_1 : POST_4;

    if (robot_state() != RETURN_TO_GAUNTLET) {
#if TESTING_ORDER_OF_EVENTS
    Serial.println("status changed");
    Serial.print("status is: ");
    Serial.println(robot_state());
#endif
        return STATE_CHANGED;
    }

    if (face_reverse_direction(RETURN_TO_GAUNTLET) == STATE_CHANGED) {
#if TESTING_ORDER_OF_EVENTS
    Serial.println("direction reversal failed");
#endif
        return STATE_CHANGED;
    }
    update_position(my_fourth_post, my_third_post);
    if (reach_adjacent_location_on_tape(my_third_post, RETURN_TO_GAUNTLET, false) == STATE_CHANGED) {
        return STATE_CHANGED;
    }
    update_position(my_third_post, my_second_post);

    if (reach_adjacent_location_on_tape(my_second_post, RETURN_TO_GAUNTLET, false)) {
        return STATE_CHANGED;
    }
    update_position(my_second_post, my_first_post);
    if (reach_adjacent_location_on_tape(my_first_post, RETURN_TO_GAUNTLET, false)) {
        return STATE_CHANGED;
    }
    update_position(my_first_post, my_intersection);

    // move and turn into intersection
    if (follow_tape_till_branch(RETURN_TO_GAUNTLET) == STATE_CHANGED) {
        return STATE_CHANGED;
    }

    int direction = inevitable ? RIGHT : LEFT;
    if (turn_onto_branch(direction, RETURN_TO_GAUNTLET) == STATE_CHANGED)
    {
        return STATE_CHANGED;
    }
    update_position(my_intersection, my_gauntlet);

    if (follow_tape_till_branch(RETURN_TO_GAUNTLET) == STATE_CHANGED) {
        return STATE_CHANGED;
    }
    
    direction = inevitable ? LEFT : RIGHT;
    if (turn_onto_branch(direction, RETURN_TO_GAUNTLET)) {
        return STATE_CHANGED;
    }
    return SUCCESS;
}

int return_from_upper_post_and_intersect()
{
    // this is the case where we've hit up 3 posts and life is chill. 
    bool inevitable = run_status.bot_identity == THANOS;
    location my_gauntlet = inevitable ? THANOS_GAUNTLET : METHANOS_GAUNTLET;
    location my_intersection = inevitable ? THANOS_INTERSECTION : METHANOS_INTERSECTION;
    location my_first_post = inevitable ? POST_5 : POST_6;
    location my_second_post = inevitable ? POST_5 : POST_5;

    if (robot_state() != RETURN_TO_GAUNTLET) {
#if TESTING_ORDER_OF_EVENTS
    Serial.println("status changed");
    Serial.print("status is: ");
    Serial.println(robot_state());
#endif
        return STATE_CHANGED;
    }

    if (face_reverse_direction(RETURN_TO_GAUNTLET) == STATE_CHANGED) {
#if TESTING_ORDER_OF_EVENTS
    Serial.println("direction reversal failed");
#endif
        return STATE_CHANGED;
    }
    update_position(my_second_post, my_first_post);
    if (reach_adjacent_location_on_tape(my_first_post, RETURN_TO_GAUNTLET, false) == STATE_CHANGED) {
        return STATE_CHANGED;
    }
    update_position(my_first_post, my_intersection);

    // move and turn into intersection
    if (follow_tape_till_branch(RETURN_TO_GAUNTLET) == STATE_CHANGED) {
        return STATE_CHANGED;
    }

    int direction = inevitable ? LEFT : RIGHT;
    if (turn_onto_branch(direction, RETURN_TO_GAUNTLET) == STATE_CHANGED)
    {
        return STATE_CHANGED;
    }
    update_position(my_intersection, my_gauntlet);

    if (follow_tape_till_branch(RETURN_TO_GAUNTLET) == STATE_CHANGED) {
        return STATE_CHANGED;
    }
    
    direction = inevitable ? LEFT : RIGHT;
    if (turn_onto_branch(direction, RETURN_TO_GAUNTLET)) {
        return STATE_CHANGED;
    }
    return SUCCESS;
}

