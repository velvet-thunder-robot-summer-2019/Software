#include "DecisionMaking/Navigation.h"
#include "AllPurposeInclude.h"
#include "GlobalInfo/Interrupts.h"
#include "Debugging/Menu.h"

#define UP_RAMP_ENCODER_DT 10 // ms, dummy val rn
#define ENCODER_DT_DELTA 4 // ms
#define TURN_TIME 200 //ms
#define RESTART_TIME 700 // ms
#define STOP_TIME 20 // ms
#define BACKUP_TIME 800 // ms

int face_reverse_direction(state expected_state);
int reach_adjacent_location_on_tape(location next_location, state expected_state);
String get_location_string(location my_loc);
void print_position();
int turn_around(state expected_state);

int reach_adjacent_location_on_tape(location next_location, state expected_state, bool stopping_at_branch)
{ 
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
    } else {
        uint32_t start_time = millis();
        while (millis() - start_time < RESTART_TIME) {
            follow_tape(FLAT_GROUND_TAPE_FOLLOWING_PWM);
        }
    }
    return SUCCESS;
}


/**
 * Turns onto the LEFT or RIGHT path of a fork (direction specified by param)
 * Params:      direction - LEFT or RIGHT, this is the fork direction we want
 */
int turn_onto_branch(int direction, state expected_state)
{
#if TESTING_ORDER_OF_EVENTS
Serial.print("we are turning ");
if (direction == LEFT) {
    Serial.println("left");
} else {
    Serial.println("right");
}
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
    stop_motors();
    return SUCCESS;
}

/**
 * Works exclusively on tape. Turns the bot around 180 degrees to return from when it cames, then stops it
 */
int face_reverse_direction(state expected_state)
{
#if TESTING_ORDER_OF_EVENTS
    Serial.println("face reverse direction, turn around");
    delay(1000);
    return SUCCESS;
#endif
    int direction;
    int stop_direction;
    int (*outer_sensor)();
    if ((run_status.bot_identity == THANOS && run_status.target_branch == LOWER) ||
        (run_status.bot_identity == METHANOS && run_status.target_branch == UPPER)) {
        direction = RIGHT;
        stop_direction = LEFT;
        outer_sensor = outer_right_sensor;
    } else {
        direction = LEFT;
        stop_direction = RIGHT;
        outer_sensor = outer_left_sensor;
    }
    rotate_on_spot(TURN_PWM, direction);
    // uint32_t start_time = millis();
    // while (millis() - start_time < 1000) {
    //     get_tape_following_error();
    //     if (robot_state() != expected_state) {
    //         return STATE_CHANGED;
    //     }
    // }
    while (!outer_sensor()) {
        // turn fairly fast until then
        get_tape_following_error();
        if (robot_state() != expected_state) {
            return STATE_CHANGED;
        }
    }
    Serial.println("outer sensor found");
    rotate_on_spot(TURN_PWM * 0.8, direction);
    while (!(inner_left_sensor() || inner_right_sensor())) {
        // turn slower around
        get_tape_following_error();
        if (robot_state() != expected_state) {
            return STATE_CHANGED;
        }
    }
    // stop motion once on line
    uint32_t start_time = millis();
    rotate_on_spot(1, stop_direction);
    while (millis() - start_time < STOP_TIME) {
        get_tape_following_error();
    }
    rotate_on_spot(0, LEFT);

    if (robot_state() != expected_state) {
        return STATE_CHANGED;
    }
    // flip robot positioning system
    return SUCCESS;
}

int return_to_intersection(state expected_state)
{
    bool inevitable = (run_status.bot_identity == THANOS);
    location my_intersection = inevitable ? THANOS_INTERSECTION : METHANOS_INTERSECTION;
    location other_intersection = inevitable ? METHANOS_INTERSECTION : THANOS_INTERSECTION;

    turn_around(expected_state);
#if DEBUG_SCREEN_DELAYS
    display_string("we've turned");
#endif
    if (run_status.target_branch == UPPER) {
        location my_first_post = inevitable ? POST_5 : POST_6;
        location my_second_post = inevitable ? POST_6 : POST_5;
        location my_intersection = inevitable ? THANOS_INTERSECTION : METHANOS_INTERSECTION;
        
        if (run_status.bot_position.last_location == my_second_post && run_status.bot_position.next_location == other_intersection) {
            update_position(my_second_post, my_first_post);
        }
        if (run_status.bot_position.last_location == my_second_post && run_status.bot_position.next_location == my_first_post) {
            if (reach_adjacent_location_on_tape(my_first_post, RETURN_TO_GAUNTLET, false) == STATE_CHANGED) {
                return STATE_CHANGED;
            }
            update_position(my_first_post, my_intersection);
        }
        if (run_status.bot_position.last_location ==  my_first_post && run_status.bot_position.next_location == my_second_post) {
            update_position(my_first_post, my_intersection);
        }

        // move and turn into intersection
        if (follow_tape_till_branch(RETURN_TO_GAUNTLET, FLAT_GROUND_TAPE_FOLLOWING_PWM) == STATE_CHANGED) {
            return STATE_CHANGED;
        }
    } else {
        location my_first_post = inevitable ? POST_4 : POST_1;
        location my_second_post = inevitable ? POST_3 : POST_2;
        location my_third_post = inevitable ? POST_2 : POST_3;
        location my_fourth_post = inevitable ? POST_1 : POST_4;
        
        if (run_status.bot_position.last_location == my_fourth_post && run_status.bot_position.next_location == other_intersection) {
            update_position(my_fourth_post, my_third_post);
#if DEBUG_SCREEN_DELAYS
            display_string("4th-3rd");
#endif
        }
        if (run_status.bot_position.last_location == my_fourth_post && run_status.bot_position.next_location == my_third_post) {
            if (reach_adjacent_location_on_tape(my_third_post, expected_state, true) == STATE_CHANGED) {
                return STATE_CHANGED;
            }
            update_position(my_third_post, my_second_post);
#if DEBUG_SCREEN_DELAYS
            display_string("3rd-2nd");
#endif
        }
        if (run_status.bot_position.last_location == my_third_post && run_status.bot_position.next_location == my_second_post) {
            if (reach_adjacent_location_on_tape(my_second_post, expected_state, true)) {
                return STATE_CHANGED;
            }
            // delay(500);
            update_position(my_second_post, my_first_post);
#if DEBUG_SCREEN_DELAYS
            display_string("2nd-1st");
#endif
        }
        if (run_status.bot_position.last_location == my_second_post && run_status.bot_position.next_location == my_first_post) {
            if (reach_adjacent_location_on_tape(my_first_post, expected_state, true)) {
                return STATE_CHANGED;
            }
            // delay(500);
            update_position(my_first_post, my_intersection);
#if DEBUG_SCREEN_DELAYS
            display_string("1st-inter");
#endif
        }

        // move and turn into intersection
        if (follow_tape_till_branch(expected_state, FLAT_GROUND_TAPE_FOLLOWING_PWM) == STATE_CHANGED) {
            return STATE_CHANGED;
        }
    }
    return SUCCESS;
}

int turn_around(state expected_state)
{
    reverse(FLAT_GROUND_TAPE_FOLLOWING_PWM);
    uint32_t start_time = millis();
    while (millis() - start_time < BACKUP_TIME) {
        get_tape_following_error();
        if (robot_state() != expected_state) {
            return STATE_CHANGED;
        }
    }
    stop_motors(BACK);
    
    if (face_reverse_direction(expected_state) == STATE_CHANGED) {
#if TESTING_ORDER_OF_EVENTS
    Serial.println("direction reversal failed");
#endif
        return STATE_CHANGED;
    }
    return SUCCESS;
}

void update_position(location last_location, location next_location)
{
    run_status.bot_position.last_location = last_location;
    run_status.bot_position.next_location = next_location;
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

 