#include "AllPurposeInclude.h"
#include "DecisionMaking/DecisionMaking.h"
#include "StateControl/FindPost.h"

int debug_iterations = 0;

void find_post()
{
#if TESTING_ORDER_OF_EVENTS
    Serial.println("");
    Serial.println("");
    Serial.println("FIND_POST state entered!");
    Serial.println("______________________");
#endif
    bool I_am_inevitable = run_status.bot_identity == THANOS;
    location my_gauntlet = I_am_inevitable ? THANOS_GAUNTLET : METHANOS_GAUNTLET;
    location my_intersection = I_am_inevitable ? THANOS_INTERSECTION : METHANOS_INTERSECTION;

    location my_first_post = I_am_inevitable ? POST_4 : POST_1;
    location my_second_post = I_am_inevitable ? POST_3 : POST_2;
    location my_third_post = I_am_inevitable ? POST_2 : POST_3;
    location my_fourth_post = I_am_inevitable ? POST_1 : POST_4;


    if (run_status.bot_position.last_location == my_gauntlet && run_status.bot_position.next_location == my_intersection)
    {
        int post_index = I_am_inevitable ? 3 : 0; // this should always be true for now

        if (run_status.stones_status[post_index] == UNKNOWN) { // if we haven't been to POST_4 (THANOS), POST_1 (METHANOS). SHOULD BE TRUE
            // go to POST_4 / POST_1

            // tape follow till fork in road
            while (!branch_reached_front()) {
                follow_tape(FLAT_GROUND_TAPE_FOLLOWING_PWM);
                if (robot_state() != FIND_POST) {
                    return;
                }
            }

            // turn the correct direction at the fork
            int turn_direction = I_am_inevitable ? LEFT : RIGHT;
            if (turn_onto_branch(turn_direction, FIND_POST) == STATE_CHANGED) {
                return;
            }

            update_position(my_intersection, my_first_post);

            // reach next location
            if (reach_adjacent_location_on_tape(my_first_post, FIND_POST, TRUE) == STATE_CHANGED) {
                return;
            }
            update_position(my_first_post, my_second_post);
        } else {
            digitalWrite(BLINKY, HIGH); // error bad things happened
            delay(1000);
            digitalWrite(BLINKY, LOW);
            delay(1000);
            digitalWrite(BLINKY, HIGH);
            delay(1000);
            return;
        }
    } else if (run_status.bot_position.last_location == my_first_post && run_status.bot_position.next_location == my_second_post)
    {
        int post_index = I_am_inevitable ? 2 : 1;

        if (run_status.stones_status[post_index] == UNKNOWN) {
            // reach next location
            if (reach_adjacent_location_on_tape(my_second_post, FIND_POST, TRUE) == STATE_CHANGED) {
                return;
            }
            update_position(my_second_post, my_third_post);
        } else {
            digitalWrite(BLINKY, HIGH); // error bad things happened
            delay(1000);
            digitalWrite(BLINKY, LOW);
            delay(1000);
            digitalWrite(BLINKY, HIGH);
            delay(1000);
            digitalWrite(BLINKY, LOW);
            return;
        }
    } else if (run_status.bot_position.last_location == my_second_post && run_status.bot_position.next_location == my_third_post) 
    {
        int post_index = I_am_inevitable ? 1 : 2;

        if (run_status.stones_status[post_index] == UNKNOWN) {
            // reach next location
            if (reach_adjacent_location_on_tape(my_third_post, FIND_POST, TRUE) == STATE_CHANGED) {
                return;
            }
            update_position(my_third_post, my_fourth_post);
        } else {
            digitalWrite(BLINKY, HIGH); // error bad things happened
            delay(1000);
            digitalWrite(BLINKY, LOW);
            delay(1000);
            digitalWrite(BLINKY, HIGH);
            delay(1000);
            digitalWrite(BLINKY, LOW);
            return;
        }
    } else if (run_status.bot_position.last_location == my_third_post && run_status.bot_position.next_location == my_fourth_post) 
    {
        int post_index = I_am_inevitable ? 1 : 2;

        if (run_status.stones_status[post_index] == UNKNOWN) {
            // reach next location
            if (reach_adjacent_location_on_tape(my_fourth_post, FIND_POST, TRUE) == STATE_CHANGED) {
                return;
            }
            update_position(my_fourth_post, I_am_inevitable ? METHANOS_INTERSECTION : THANOS_INTERSECTION);
        } else {
            digitalWrite(BLINKY, HIGH); // error bad things happened
            delay(1000);
            digitalWrite(BLINKY, LOW);
            delay(1000);
            digitalWrite(BLINKY, HIGH);
            delay(1000);
            digitalWrite(BLINKY, LOW);
            return;
        }
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
        if (switch_state(FIND_POST, RETURN_TO_GAUNTLET) == OTHER_STATE_CHANGE_OCCURRED_FIRST) {
            Serial.println("other state changed preceeded this one");
        }
    }

    if (robot_state() != FIND_POST) {
#if TESTING_ORDER_OF_EVENTS
        Serial.println("Robot state has changed from find_post");
        Serial.print("the state is: ");
        Serial.println(robot_state());
#endif
        return;
    }

    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(FIND_POST, GET_INFINITY_STONE);
    } else {
        switch_state(FIND_POST, MENU);
    }
}
