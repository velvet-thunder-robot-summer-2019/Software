#ifndef NAVIGATION_H_
#define NAVIGATION_H_

#include "GlobalInfo/GlobalVars.h"

int ramp_reached();
void move_to(location next_location);
void update_position(location last_location, location next_location);
int reach_adjacent_location_on_tape(location next_location, state expected_state, bool stopping_at_branch);
int face_reverse_direction(state expected_state);

/**
 * Turns onto the LEFT or RIGHT path of a fork (direction specified by param)
 * Params:      direction - LEFT or RIGHT, this is the fork direction we want
 */
int turn_onto_branch(int direction, state expected_state);

int follow_tape_till_branch(state expected_state, float pwm);

/**
 * Turns onto the LEFT or RIGHT path of a fork, into the gauntlet
 * Params:      direction - LEFT or RIGHT, this is the gauntlet direction
 */
int turn_into_gauntlet(int direction, state expected_state);

/**
 * Returns side on which post / branch is expected to be found
 */
int get_branch_side(location next);

int return_to_intersection(state expected_state);

#endif // NAVIGATION_H_