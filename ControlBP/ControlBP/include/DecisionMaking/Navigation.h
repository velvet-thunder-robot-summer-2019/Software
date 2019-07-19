#ifndef POSITIONING_H_
#define POSITIONING_H_

#include "GlobalInfo/GlobalVars.h"

int ramp_reached();
void move_to(location next_location);
void update_position(location last_location, location next_location);
int reach_adjacent_location_on_tape(location next_location, state expected_state, bool stopping_at_branch);

/**
 * Turns onto the LEFT or RIGHT path of a fork (direction specified by param)
 * Params:      direction - LEFT or RIGHT, this is the fork direction we want
 */
int turn_onto_branch(int direction, state expected_state);

#endif // POSITIONING_H_