#ifndef ROBOT_STATE_H_
#define ROBOT_STATE_H_

#include "GlobalVars.h"

/**
 * inits the robot state
 */
void init_robot_state(state start_state);

/**
 * Switches the bot state 
 * Params:      current_state - state that the function caller believes the robot to be in
 *              new_state     - state to place the robot into
 * Returns:     TRUE if current_state was actually the state of the robot
 *              FALSE otherwise
 *              
 */
int switch_state(state current_state, state new_state);

/**
 * Returns: the robot state (e.g. REACH_RAMP, GET_INFINITY_STONE etc)
 */
state robot_state();

/**
 * Returns: the previous robot state, state that came before the current one (e.g. REACH_RAMP, GET_INFINITY_STONE etc)
 */
state previous_robot_state();

#endif //ROBOT_STATE_H_