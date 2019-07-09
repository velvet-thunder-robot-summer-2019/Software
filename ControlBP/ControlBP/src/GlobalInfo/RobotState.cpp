#include "AllPurposeInclude.h"
#include <mutex>

static state bot_state;
static state previous_bot_state;
// static std::mutex state_mutex;

/**
 * Inits the robot state
 */
void init_robot_state(state start_state)
{
    bot_state = start_state;
    previous_bot_state = MENU;
}

/**
 * Switches the bot state 
 * Params:      current_state - state that the function caller believes the robot to be in
 *              new_state     - state to place the robot into
 * Returns:     TRUE if current_state was actually the state of the robot
 *              FALSE otherwise
 *              
 */
int switch_state(state current_state, state new_state)
{
    /*
    if (state_mutex.try_lock()) {
        if (current_state == bot_state) {
            previous_bot_state = bot_state;
            bot_state = new_state;
            return STATE_CHANGE_SUCCESSFUL;
        }
        return OTHER_STATE_CHANGE_OCCURRED_FIRST;
    }
    return OTHER_STATE_CHANGE_IN_PROGRESS;
    */
    if (current_state == bot_state) {
            previous_bot_state = bot_state;
            bot_state = new_state;
            return STATE_CHANGE_SUCCESSFUL;
        }
    return OTHER_STATE_CHANGE_OCCURRED_FIRST;
}

/**
 * Returns: the robot state (e.g. REACH_RAMP, GET_INFINITY_STONE etc)
 */
state robot_state()
{
    return bot_state;
}

/**
 * Returns: the previous robot state, state that came before the current one (e.g. REACH_RAMP, GET_INFINITY_STONE etc)
 */
state previous_robot_state()
{
    return previous_bot_state;
}
