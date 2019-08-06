#include "AllPurposeInclude.h"
#include "DecisionMaking/DecisionMaking.h"
#include "StateControl/FindPost.h"
#include "Debugging/Menu.h"
#include "StateControl/HandleCollision.h"

int follow_lower_path();
int follow_upper_path();

int debug_iterations = 0;

void find_post()
{
#if TESTING_ORDER_OF_EVENTS
    Serial.println("");
    Serial.println("");
    Serial.println("FIND_POST state entered!");
    Serial.println("______________________");
#endif

if (millis() - run_status.comp_start_time < TIME_TO_TARGET_NEW_POST) {
    switch_state(FIND_POST, RETURN_TO_GAUNTLET);
    return;
}

if (switch_branch_needed()) {
    int turn_direction;
    bool inevitable = (run_status.bot_identity == THANOS);
    if (run_status.target_branch == UPPER) {
        turn_direction = inevitable ? RIGHT : LEFT;
    } else {
        turn_direction = inevitable ? LEFT : RIGHT;
    }
    return_to_intersection(FIND_POST);
    turn_onto_branch(turn_direction, FIND_POST);
    set_switch_branch_false();
}


if (run_status.target_branch == UPPER) {
    follow_upper_path();
} else {
    follow_lower_path();
}
    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(FIND_POST, GET_INFINITY_STONE);
    } else {
        switch_state(FIND_POST, GET_INFINITY_STONE);
    }
}

int follow_upper_path()
{
    bool thanos = run_status.bot_identity == THANOS;
    location my_gauntlet = thanos ? THANOS_GAUNTLET : METHANOS_GAUNTLET;
    location my_intersection = thanos ? THANOS_INTERSECTION : METHANOS_INTERSECTION;

    location my_first_post = thanos ? POST_5 : POST_6;
    location my_second_post = thanos ? POST_6 : POST_5;
    location opp_intersection = thanos ? METHANOS_INTERSECTION : THANOS_INTERSECTION;
    if (run_status.bot_position.last_location == my_gauntlet && run_status.bot_position.next_location == my_intersection)
    {
            // tape follow till fork in road
            if (follow_tape_till_branch(FIND_POST, FLAT_GROUND_TAPE_FOLLOWING_PWM) == STATE_CHANGED) {
                return STATE_CHANGED;
            }

        // turn the correct direction at the fork
        int turn_direction = thanos ? RIGHT : LEFT;
#if TESTING_ORDER_OF_EVENTS
    Serial.print("the direction in which we want to turn is ");
    if (turn_direction == RIGHT) {
        Serial.println("right");
    } else {
        Serial.println("left");
    }
#endif

        if (turn_onto_branch(turn_direction, FIND_POST) == STATE_CHANGED) {
            return STATE_CHANGED;
        }
        // delay(500);

        update_position(my_intersection, my_first_post);

        // reach next location
        if (reach_adjacent_location_on_tape(my_first_post, FIND_POST, true) == STATE_CHANGED) {
            return STATE_CHANGED;
        }
        
        update_position(my_first_post, my_second_post);
    } else if (run_status.bot_position.last_location == my_intersection && run_status.bot_position.next_location == my_first_post)
    {
        if (reach_adjacent_location_on_tape(my_first_post, FIND_POST, true) == STATE_CHANGED) {
            return STATE_CHANGED;
        }
        update_position(my_first_post, my_second_post);
    } else if (run_status.bot_position.last_location == my_first_post && run_status.bot_position.next_location == my_second_post)
    {
            if (reach_adjacent_location_on_tape(my_second_post, FIND_POST, true) == STATE_CHANGED) {
                return STATE_CHANGED;
            }
            update_position(my_second_post, opp_intersection);
    } else {
        // this is temporary for the next tuning run, we just go Home
#if TESTING_ORDER_OF_EVENTS
        Serial.println("We've picked up all stones, return home");
        delay(1000);
        Serial.print("state is: ");
        Serial.print(robot_state());
#endif
        if (digitalRead(MASTER_SWITCH) == COMP) {
            switch_state(FIND_POST, RETURN_TO_GAUNTLET);
        } else {
            switch_state(FIND_POST, MENU);
        } 
        return STATE_CHANGED;
    }
    return SUCCESS;
}

int follow_lower_path()
{
    bool I_am_inevitable = run_status.bot_identity == THANOS;
    location my_gauntlet = I_am_inevitable ? THANOS_GAUNTLET : METHANOS_GAUNTLET;
    location my_intersection = I_am_inevitable ? THANOS_INTERSECTION : METHANOS_INTERSECTION;

    location my_first_post = I_am_inevitable ? POST_4 : POST_1;
    location my_second_post = I_am_inevitable ? POST_3 : POST_2;
    location my_third_post = I_am_inevitable ? POST_2 : POST_3;
    location my_fourth_post = I_am_inevitable ? POST_1 : POST_4;

    if (run_status.bot_position.last_location == my_gauntlet && run_status.bot_position.next_location == my_intersection)
    {
        // tape follow till fork in road
        if (follow_tape_till_branch(FIND_POST, FLAT_GROUND_TAPE_FOLLOWING_PWM) == STATE_CHANGED) {
            return STATE_CHANGED;
        }

        // turn the correct direction at the fork
        int turn_direction = I_am_inevitable ? LEFT : RIGHT;
#if TESTING_ORDER_OF_EVENTS
        Serial.print("the direction in which we want to turn is ");
        if (turn_direction == RIGHT) {
            Serial.println("right");
        } else {
            Serial.println("left");
        }
#endif
        if (turn_onto_branch(turn_direction, FIND_POST) == STATE_CHANGED) {
            // return STATE_CHANGED;
        }
        // delay(500);

        update_position(my_intersection, my_first_post);
#if DEBUG_SCREEN_DELAYS
        display_string("pos updated");
#endif
            // reach next location
        if (reach_adjacent_location_on_tape(my_first_post, FIND_POST, true) == STATE_CHANGED) {
#if DEBUG_SCREEN_DELAYS
        display_string("reached next loc");
#endif
            // return STATE_CHANGED;
        }
        update_position(my_first_post, my_second_post);
    } else if (run_status.bot_position.last_location == my_intersection && run_status.bot_position.next_location == my_first_post)
    {
        if (reach_adjacent_location_on_tape(my_first_post, FIND_POST, true) == STATE_CHANGED) {
            // return STATE_CHANGED;
        }
        update_position(my_first_post, my_second_post);
    } else if (run_status.bot_position.last_location == my_first_post && run_status.bot_position.next_location == my_second_post)
    {
        if (reach_adjacent_location_on_tape(my_second_post, FIND_POST, true) == STATE_CHANGED) {
            // return STATE_CHANGED;
        }
        update_position(my_second_post, my_third_post);
    } else if (run_status.bot_position.last_location == my_second_post && run_status.bot_position.next_location == my_third_post) 
    {
        if (reach_adjacent_location_on_tape(my_third_post, FIND_POST, true) == STATE_CHANGED) {
            // return STATE_CHANGED;
        }
        update_position(my_third_post, my_fourth_post);
    } else if (run_status.bot_position.last_location == my_third_post && run_status.bot_position.next_location == my_fourth_post) 
    {
        if (reach_adjacent_location_on_tape(my_fourth_post, FIND_POST, true) == STATE_CHANGED) {
            // return STATE_CHANGED;
        }
        update_position(my_fourth_post, I_am_inevitable ? METHANOS_INTERSECTION : THANOS_INTERSECTION);
    } else 
    {
        // this is temporary for the next tuning run, we just go Home
#if TESTING_ORDER_OF_EVENTS
        Serial.println("We've picked up all stones, return home");
        digitalWrite(BLINKY, HIGH); // error bad things happened
        delay(1000);
        digitalWrite(BLINKY, LOW);
        delay(1000);
        digitalWrite(BLINKY, HIGH);
        Serial.print("state is: ");
        Serial.print(robot_state());
#endif
        if (digitalRead(MASTER_SWITCH) == COMP) {
            switch_state(FIND_POST, RETURN_TO_GAUNTLET);
        } else {
            switch_state(FIND_POST, MENU);
        }   
        return STATE_CHANGED;
    }

    return SUCCESS;
}
