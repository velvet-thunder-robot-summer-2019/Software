#include "DecisionMaking/Navigation.h"
#include "AllPurposeInclude.h"
#include "GlobalInfo/Interrupts.h"
#include "Debugging/Menu.h"

#define UP_RAMP_ENCODER_DT 10 // ms, dummy val rn
#define ENCODER_DT_DELTA 4 // ms
#define TURN_TIME 200 //ms
#define RESTART_TIME 700 // ms
#define STOP_TIME 20 // ms

location target_location = nullptr; // nullptr if we're not going anywhere

int face_reverse_direction(state expected_state);
int reach_adjacent_location_on_tape(location next_location, state expected_state);
int branch_side_expected(location next_location);
String get_location_string(location my_loc);
void print_position();
int get_branch_side(location next);

int ramp_reached()
{
    // TODO: implement based on encoder speed
    uint32_t avg_velocity_left = (left_wheel_dt[0] + left_wheel_dt[1] + left_wheel_dt[2]) / 3;
    uint32_t avg_velocity_right = (right_wheel_dt[0] + right_wheel_dt[1] + right_wheel_dt[2]) / 3;
#if TESTING_ORDER_OF_EVENTS
    Serial.println("ramp_reached method");
    Serial.print("avg velocity left: ");
    Serial.println(avg_velocity_left);
    Serial.print("avg velocity right");
    Serial.println(avg_velocity_right);
    return TRUE;
#endif
    
    if ((abs((int) (UP_RAMP_ENCODER_DT - avg_velocity_left)) < ENCODER_DT_DELTA) && 
        (abs((int)(UP_RAMP_ENCODER_DT - avg_velocity_right) < ENCODER_DT_DELTA)))
    {
        return TRUE;
    }
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

location get_target_location()
{
    return target_location;
}

int reach_adjacent_location_on_tape(location next_location, state expected_state, bool stopping_at_branch)
{ 
    target_location = next_location;
#if TESTING_ORDER_OF_EVENTS
    Serial.println("reaching next location on tape");
    delay(1000);
    return SUCCESS;
#endif
    // turn around if need be  
    if (run_status.bot_position.last_location == next_location) {
        int reversal_success = face_reverse_direction(expected_state);
        if (reversal_success == STATE_CHANGED) {
            return STATE_CHANGED;
        }
    }

    uint32_t start_time = millis();
    while (millis() - start_time < RESTART_TIME) {
        follow_tape(FLAT_GROUND_TAPE_FOLLOWING_PWM);
    }

    int front_reached_branch = branch_reached_front();

    while (!front_reached_branch) {// && !back_reached_branch) {
        follow_tape(FLAT_GROUND_TAPE_FOLLOWING_PWM);
        if (robot_state() != expected_state) {
            return STATE_CHANGED;
        }
        front_reached_branch = branch_reached_front();
    }
    if (stopping_at_branch) {
        stop_motors();
    }
    target_location = nullptr;
    return SUCCESS;
}


/**
 * Turns onto the LEFT or RIGHT path of a fork (direction specified by param)
 * Params:      direction - LEFT or RIGHT, this is the fork direction we want
 */
int turn_onto_branch(int direction, state expected_state)
{
#if TESTING_ORDER_OF_EVENTS
Serial.println("we are turning on to the branch...");
delay(1000);
Serial.println("turn completed!");
return SUCCESS;
#endif

    bool go_left = (direction == LEFT);

    int (*const inner_sensor_on_turn_side)() = go_left ? inner_left_sensor : inner_right_sensor;
    int (*const inner_sensor_away_from_side)() = go_left ? inner_right_sensor : inner_left_sensor;


    int inner_turn_side = go_left ? last_stop_left() : last_stop_right(); 
    int inner_away_side = go_left ? last_stop_right() : last_stop_left();
    int outer_turn_side = go_left ? last_stop_outer_left() : last_stop_outer_right();
    int outer_away_side =  go_left ? last_stop_outer_right() : last_stop_outer_left();
    if (robot_state() != expected_state) {
        return STATE_CHANGED;
    }
    String if_case = "0";
    // stop_motors();
    if (outer_turn_side && (inner_turn_side || inner_away_side)) {
        if_case = "1";
        follow_arc_rho(direction, ARC_LENGTH_FOR_TURN, TURN_PWM);
        while (inner_sensor_on_turn_side() || inner_sensor_away_from_side()) {
            if (robot_state() != expected_state) {
                return STATE_CHANGED;
            }
        get_tape_following_error();
        }
        // follow_arc_rho(direction, ARC_LENGTH_FOR_TURN, TURN_PWM);
        uint32_t start_time = millis();
        while (millis() - start_time < TURN_TIME) {
            if (robot_state() != expected_state) {
                return STATE_CHANGED;
            }
            get_tape_following_error();
        }
        // follow_arc_rho(direction, ARC_LENGTH_FOR_TURN, TURN_PWM);
        while (!inner_sensor_on_turn_side() || !inner_sensor_away_from_side()) {
            if (robot_state() != expected_state) {
                return STATE_CHANGED;
            }
            get_tape_following_error();
        }
    } else if (outer_away_side && (inner_turn_side || inner_away_side)) {
        if_case = "2";
        // much rejoicing
    } else if (outer_turn_side) {
        if_case = "3";
        follow_arc_rho(direction, ARC_LENGTH_FOR_TURN, TURN_PWM);
        uint32_t start_time = millis();
        while (millis() - start_time < TURN_TIME) {
            if (robot_state() != expected_state) {
                return STATE_CHANGED;
            }
            get_tape_following_error();
        }
        // follow_arc_rho(direction, ARC_LENGTH_FOR_TURN, TURN_PWM);
        while (!inner_sensor_on_turn_side() && !inner_sensor_away_from_side()) {
            if (robot_state() != expected_state) {
                return STATE_CHANGED;
            }
            get_tape_following_error();
        }
    } else if (outer_away_side) {
        if_case = "4";
        follow_arc_rho(direction, ARC_LENGTH_FOR_TURN, TURN_PWM);
        while (!inner_sensor_on_turn_side() && !inner_sensor_away_from_side()) {
            if (robot_state() != expected_state) {
                return STATE_CHANGED;
            }
            get_tape_following_error();
        }
    }
    stop_motors();
#if DEBUG_SCREEN_DELAYS
    display_string(if_case);
#endif
   return SUCCESS;
}


/**
 * Turns onto the LEFT or RIGHT path of a fork, into the gauntlet
 * Params:      direction - LEFT or RIGHT, this is the gauntlet direction
 */
int turn_into_gauntlet(int direction, state expected_state)
{
#if TESTING_ORDER_OF_EVENTS
Serial.println("following tape till branch, remove this if later!!");
return SUCCESS;
#endif
    bool go_left = (direction == LEFT);

    int (*const inner_sensor_on_turn_side)() = go_left ? inner_left_sensor : inner_right_sensor;
    int (*const inner_sensor_away_from_side)() = go_left ? inner_right_sensor : inner_left_sensor;


    int inner_turn_side = go_left ? last_stop_left() : last_stop_right(); 
    int inner_away_side = go_left ? last_stop_right() : last_stop_left();
    int outer_turn_side = go_left ? last_stop_outer_left() : last_stop_outer_right();
    int outer_away_side =  go_left ? last_stop_outer_right() : last_stop_outer_left();
    if (robot_state() != expected_state) {
        return STATE_CHANGED;
    }
    String if_case = "0";
    // stop_motors();
    if (outer_turn_side && (inner_turn_side || inner_away_side)) {
        if_case = "1";
        rotate_on_spot(TURN_PWM, direction);
        while (inner_sensor_on_turn_side() || inner_sensor_away_from_side()) {
            if (robot_state() != expected_state) {
                return STATE_CHANGED;
            }
        get_tape_following_error();
        }
        // follow_arc_rho(direction, ARC_LENGTH_FOR_TURN, TURN_PWM);
        uint32_t start_time = millis();
        while (millis() - start_time < TURN_TIME) {
            if (robot_state() != expected_state) {
                return STATE_CHANGED;
            }
            get_tape_following_error();
        }
        // follow_arc_rho(direction, ARC_LENGTH_FOR_TURN, TURN_PWM);
        while (!inner_sensor_on_turn_side() || !inner_sensor_away_from_side()) {
            if (robot_state() != expected_state) {
                return STATE_CHANGED;
            }
            get_tape_following_error();
        }
    } else if (outer_away_side && (inner_turn_side || inner_away_side)) {
        if_case = "2";
        // much rejoicing
    } else if (outer_turn_side) {
        if_case = "3";
        rotate_on_spot(TURN_PWM, direction);
        uint32_t start_time = millis();
        while (millis() - start_time < TURN_TIME) {
            if (robot_state() != expected_state) {
                return STATE_CHANGED;
            }
            get_tape_following_error();
        }
        // follow_arc_rho(direction, ARC_LENGTH_FOR_TURN, TURN_PWM);
        while (!inner_sensor_on_turn_side() && !inner_sensor_away_from_side()) {
            if (robot_state() != expected_state) {
                return STATE_CHANGED;
            }
            get_tape_following_error();
        }
    } else if (outer_away_side) {
        if_case = "4";
        rotate_on_spot(TURN_PWM, direction);
        while (!inner_sensor_on_turn_side() && !inner_sensor_away_from_side()) {
            if (robot_state() != expected_state) {
                return STATE_CHANGED;
            }
            get_tape_following_error();
        }
    }
    stop_motors();
#if DEBUG_SCREEN_DELAYS
    display_string(if_case);
#endif
   return SUCCESS;
}


int follow_tape_till_branch(state expected_state, float PWM) {
#if TESTING_ORDER_OF_EVENTS
Serial.println("follow_tape_till_branch");
#endif
    while (!branch_reached_front()) {
        follow_tape(PWM);
        if (robot_state() != expected_state) {
            return STATE_CHANGED;
        }
    }
    return SUCCESS;
}

/**
 * Works exclusively on tape. Turns the bot around 180 degrees to return from when it cames, then stops it
 */
int face_reverse_direction(state expected_state)
{
#if TESTING_ORDER_OF_EVENTS
delay(1000);
return SUCCESS;
#endif
    rotate_on_spot(TURN_PWM, LEFT);
    while (!outer_left_sensor()) {
        // turn fairly fast until then
        get_tape_following_error();
        if (robot_state() != expected_state) {
            return STATE_CHANGED;
        }
    }
    rotate_on_spot(TURN_PWM * 0.8, LEFT);
    while (!(inner_left_sensor() && inner_right_sensor())) {
        // turn slower around
        get_tape_following_error();
        if (robot_state() != expected_state) {
            return STATE_CHANGED;
        }
    }
    // stop motion once on line

    uint32_t start_time = millis();
    rotate_on_spot(1, RIGHT);
    while (millis() - start_time < STOP_TIME) {
        get_tape_following_error();
    }
    rotate_on_spot(0, LEFT);

    if (robot_state() != expected_state) {
        return STATE_CHANGED;
    }
    // flip robot positioning system
    update_position(run_status.bot_position.next_location, run_status.bot_position.last_location);
    return SUCCESS;
}

void update_position(location last_location, location next_location)
{
    run_status.bot_position.last_location = last_location;
    run_status.bot_position.next_location = next_location;
    run_status.bot_position.left_wheel_ticks = 0;
    run_status.bot_position.right_wheel_ticks = 0;
#if TESTING_ORDER_OF_EVENTS
print_position();
#endif
}

void print_position()
{
    String last_loc = get_location_string(run_status.bot_position.last_location);
    String next_loc = get_location_string(run_status.bot_position.next_location);
    Serial.println("_______location info_________");
    Serial.print("last location: ");
    Serial.println(last_loc);
    Serial.print("next location: ");
    Serial.println(next_loc);
}

String get_location_string(location my_loc)
{    
    String loc;
    switch (my_loc) {
        case METHANOS_GAUNTLET:
            loc = "Methanos gauntlet";
            break;
        case METHANOS_INTERSECTION:
            loc = "Methanos intersection";
            break;
        case POST_1:
            loc = "post 1";
            break;
        case POST_2:
            loc = "post 2";
            break;
        case POST_3:
            loc = "post 3";
            break;
        case POST_4:
            loc = "post 4";
            break;
        case POST_5:
            loc = "post 5";
            break;
        case POST_6:
            loc = "post 6";
            break;
        case THANOS_GAUNTLET:
            loc = "Thanos gauntlet";
            break;
        case THANOS_INTERSECTION:
            loc = "Thanos intersection";
            break;
        case METHANOS_START:
            loc = "Methanos start";
            break;
        default:
            loc = "Thanos start";
            break;
    }
    return loc;
}

int get_branch_side(location next)
{
    location last = run_status.bot_position.last_location;
    if ((last == THANOS_INTERSECTION && next == POST_4) ||
        (last == POST_4 && next == POST_3) ||
        (last == POST_3 && next == POST_2) ||
        (last == POST_2 && next == POST_1) ||
        (last == POST_1 && next == THANOS_INTERSECTION) ||
        (last == METHANOS_INTERSECTION && next == POST_6) ||
        (last == POST_6 && next == POST_5) ||
        (last == POST_5 && next == THANOS_INTERSECTION)) {
            return LEFT;
    } 
    if ((last == THANOS_INTERSECTION && next == POST_5) ||
        (last == POST_5 && next == POST_6) ||
        (last == POST_6 && next == METHANOS_INTERSECTION) ||
        (last == METHANOS_INTERSECTION && next == POST_1) ||
        (last == POST_1 && next == POST_2) ||
        (last == POST_2 && next == POST_3) ||
        (last == POST_3 && next == POST_4) ||
        (last == POST_4 && next == THANOS_INTERSECTION)) {
            return RIGHT;
        }
    return BOTH_SIDES;
}

 