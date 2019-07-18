#include "DecisionMaking/Navigation.h"
#include "AllPurposeInclude.h"

int face_reverse_direction(state expected_state);
int reach_adjacent_location_on_tape(location next_location, state expected_state);
int branch_side_expected(location next_location);

int ramp_reached()
{
    // TODO: implement based on encoder speed
    return FALSE;
}

void move_to(location next_location)
{
    // each 'move_to' targeting a post ends when the back sensors detect the back of it
    if ((run_status.bot_position.last_location == POST_1 && run_status.bot_position.next_location == POST_2) ||
        (run_status.bot_position.last_location == POST_2 && run_status.bot_position.next_location == POST_1)) {
        // i.e if we're between posts 1 and 2
        switch(next_location) {
            case POST_1:
                // turn around (pi) if last if POST_1
                // follow_tape till next intersection
                // update position to last = 1, next = METHANOS_INTERSECTION
                break;
            case POST_2:
                // turn around (pi) if last if POST_2
                // follow tape till next intersection

                // update position to last = 2, next = 3
                break;
            case POST_3:
                move_to(POST_2);
                move_to(POST_3); //updates position to last = 3, next = 4
                break;
            case POST_4:
                move_to(POST_3);
                move_to(POST_4); // updates position to last = 4, next = 5
                break;
            case POST_5:
                move_to(POST_6);
                move_to(POST_5);
                // update position to last = 6, next = 5
                break;
            case POST_6:
                //rotate till you face "up"
                //cross country till you hit tape
                // turn to face right
                // update position to last = METHANOS_INTERSECTION, next = 6
                move_to(POST_6);
                break;
            case THANOS_INTERSECTION:
                move_to(POST_5);
                move_to(THANOS_INTERSECTION);
                break;
            case THANOS_GAUNTLET:
                move_to(THANOS_INTERSECTION);
                move_to(THANOS_GAUNTLET);
                break;
            case METHANOS_INTERSECTION:
                // turn until you face up
                // cross country till you hit tape
                // turn to face left
                // update position to prev = 6, next = methanos intersection
                move_to(METHANOS_INTERSECTION);
            case METHANOS_GAUNTLET:
                move_to(METHANOS_INTERSECTION);
                // turn up branch, follow to gauntlet
                // update position
                break;
            default: 
                Serial.println("Error, can't go to that location");
                break;
        }
    } else if ((run_status.bot_position.last_location == POST_2 && run_status.bot_position.next_location == POST_3) ||
        (run_status.bot_position.last_location == POST_3 && run_status.bot_position.next_location == POST_2)) {
        // i.e if we're between posts 2 and 3
        switch(next_location) {
            case POST_1:
                move_to(POST_2); // updates position: last = 2, next = 1
                move_to(POST_1);
                break;
            case POST_2:
                // turn around (pi) if last if POST_2
                // follow tape till next intersection
                // update position to last = 2, next = 1
                break;
            case POST_3:
                // turn around (pi) if last if POST_3
                // follow tape till next intersection
                // update position to last = 3, next = 4
                break;
            case POST_4:
                move_to(POST_3);
                move_to(POST_4);
                break;
            case POST_5:
                // rotate till you face "up" (positive y)
                // cross country till you hit tape
                // turn to face right
                // update position to last==6, next==5
                move_to(POST_5);
                break;
            case POST_6:
                //rotate till you face "up"
                //cross counTry till you hit tape
                // turn to face left
                // update position to last == 5, next == 6
                move_to(POST_6);
                break;
            case THANOS_INTERSECTION:
                move_to(POST_5);
                move_to(THANOS_INTERSECTION);
                break;
            case THANOS_GAUNTLET:
                move_to(THANOS_INTERSECTION);
                move_to(THANOS_GAUNTLET);
                break;
            case METHANOS_INTERSECTION:
                move_to(POST_6);
                move_to(METHANOS_INTERSECTION);
            case METHANOS_GAUNTLET:
                move_to(METHANOS_INTERSECTION);
                move_to(METHANOS_GAUNTLET);
                break;
            default: 
                Serial.println("Error, can't go to that location");
                break;
        }
    } else if ((run_status.bot_position.last_location == POST_3 && run_status.bot_position.next_location == POST_4) ||
        (run_status.bot_position.last_location == POST_4 && run_status.bot_position.next_location == POST_3)) {
        // i.e if we're between posts 3 and 4
        switch(next_location) {
            case POST_1:
                move_to(POST_2);
                move_to(POST_1);
                break;
            case POST_2:
                move_to(POST_3);
                move_to(POST_2);
                break;
            case POST_3:
                // turn around (pi) if last if POST_3
                // follow tape till next intersection
                // update position to last = 3, next = 2
                break;
            case POST_4:
                // turn around (pi) if last if POST_4
                // follow tape till next intersection
                // update position to last = 4, next = 5
                break;
            case POST_5:
                // rotate till you face "up" (positive y)
                // cross country till you hit tape
                // turn to face left
                // update position to last = THANOS_INTERSECTION, next = 5
                move_to(POST_5);
                break;
            case POST_6:
                move_to(POST_5);
                move_to(POST_6);
                break;
            case THANOS_INTERSECTION:
                // turn till you face "up" (positive y)
                // cross country till you hit tape
                // turn to face right
                // update position to last = 5, next = THANOS_INTERSECTION
                move_to(THANOS_INTERSECTION);
                break;
            case THANOS_GAUNTLET:
                move_to(THANOS_INTERSECTION);
                move_to(THANOS_GAUNTLET);
                break;
            case METHANOS_INTERSECTION:
                // rotate till you face "up" (positive y)
                // cross country till you hit tape
                // turn to face right
                // update position to last = 5, next = METHANOS_INTERSECTION
                move_to(METHANOS_INTERSECTION);
                break;
            case METHANOS_GAUNTLET:
                move_to(METHANOS_INTERSECTION);
                move_to(METHANOS_GAUNTLET);
                break;
            default: 
                Serial.println("Error, can't go to that location");
                break;
        }
    } else if ((run_status.bot_position.last_location == POST_1 && run_status.bot_position.next_location == METHANOS_INTERSECTION) ||
        (run_status.bot_position.last_location == METHANOS_INTERSECTION && run_status.bot_position.next_location == POST_1)) {
        // i.e if we're between post 1 and METHANOS_INTERSECTION
        switch(next_location) {
            case POST_1:
                // turn around (pi) if last is POST_1
                // follow_tape till next intersection (that's the branch really)
                // turn left (i.e. towards neg y roughly)
                // follow tape till next intersection
                // update position to last = 1, next = 2
                break;
            case POST_2:
                move_to(POST_1);
                move_to(POST_2);
                break;
            case POST_3:
                move_to(POST_2);
                move_to(POST_3);
                break;
            case POST_4:
                move_to(POST_3);
                move_to(POST_4);
                break;
            case POST_5:
                move_to(POST_6);
                move_to(POST_5);
                break;
            case POST_6:
                move_to(METHANOS_INTERSECTION);
                // turn to face right
                // update position, last == METHANOS_INTERSECTION, next == 6
                // call move_to(POST_6)
                break;
            case THANOS_INTERSECTION:
                move_to(POST_5);
                move_to(THANOS_INTERSECTION);
                break;
            case THANOS_GAUNTLET:
                move_to(THANOS_INTERSECTION);
                move_to(THANOS_GAUNTLET);
                break;
            case METHANOS_INTERSECTION:
                // turn around (pi) if last == METHANOS_INTERSECTION
                // tape follow till branch is hit
                // update position to last = METHANOS_INTERSECTION, next = METHANOS_GAUNTLET
                break;
            case METHANOS_GAUNTLET:
                move_to(METHANOS_INTERSECTION);
                move_to(METHANOS_GAUNTLET);
                break;
            default: 
                Serial.println("Error, can't go to that location");
                break;
        }
    }
}

int reach_adjacent_location_on_tape(location next_location, state expected_state, bool stopping_at_branch)
{  
    // turn around if need be  
    if (run_status.bot_position.last_location == next_location) {
        int reversal_success = face_reverse_direction(expected_state);
        if (reversal_success == STATE_CHANGED) {
            return STATE_CHANGED;
        }
    }
    // move till the point, be aware that we may overshoot
    int front_reached_branch = branch_reached_front();
    int back_reached_branch = branch_reached();
    while (!front_reached_branch && !back_reached_branch) {
        uint8_t response = follow_tape(FLAT_GROUND_TAPE_FOLLOWING_PWM);
        if (response == TAPE_NOT_FOUND) {
             backtrack_to_tape();
        }
        if (robot_state() != expected_state) {
            return STATE_CHANGED;
        }
        front_reached_branch = branch_reached_front();
        back_reached_branch = branch_reached();
    }
    while (!back_reached_branch) {
        uint8_t response;
        if (!stopping_at_branch) {
            response = follow_tape(FLAT_GROUND_APPROACHING_STOP_PWM);
        } else {
            response = follow_tape(FLAT_GROUND_TAPE_FOLLOWING_PWM);
        }
        if (response == TAPE_NOT_FOUND) {
             backtrack_to_tape();
        }
        if (robot_state() != expected_state) {
            return STATE_CHANGED;
        }
        back_reached_branch = branch_reached();
    }
    // if we've overshot, move back a bit. We'll have to tune it to a reasonable overshoot
    while (!branch_reached()) {
        reverse(FLAT_GROUND_APPROACHING_STOP_PWM);
    }
    stop_motors();
    return SUCCESS;
}

int reach_adjacent_branch_cross_country(location location_1, location location_2, state expected_state)
{
    // start by turning to face correct area
    // go forward till we hit other tape with ANY of the front sensors

}


/**
 * Works exclusively on tape. Turns the bot around 180 degrees to return from when it cames, then stops it
 */
int face_reverse_direction(state expected_state)
{
    while (!outer_left_sensor()) {
        // turn fairly fast until then
        rotate_on_spot(0.3);
        if (run_status.bot_state != expected_state) {
            return STATE_CHANGED;
        }
    }
    while (!left_sensor()) {
        // turn slower around
        rotate_on_spot(0.1);
        if (run_status.bot_state != expected_state) {
            return STATE_CHANGED;
        }
    }
    while (!(left_sensor() && right_sensor())) {
        // slow down to not overshoot!
        rotate_on_spot(0.07);
        if (run_status.bot_state != expected_state) {
            return STATE_CHANGED;
        }
    }
    // stop motion once on line
    rotate_on_spot(0);
    if (run_status.bot_state != expected_state) {
        return STATE_CHANGED;
    }
    return SUCCESS;
}

 