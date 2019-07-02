#include "Initialisation\GlobalVariables.h"
#include "Initialisation/HardwareDefs.h"
#include <Arduino.h>

void initialise_global_variables(position start_position, state start_state)
{
    bot_position = start_position; 
    bot_state = start_state;

    bot_previous_state = MENU; //previous state of robot
    int i;
    for (i = 0; i < TOTAL_NUM_STONE; i++) {
        stones_status[i] = UNKNOWN;
    }
}

void initialise_global_variables()
{
    state my_state;
    if (digitalRead(MASTER_SWITCH) == COMP)
    {
        my_state = REACH_RAMP; // state of robot
    } else {
        my_state = MENU;
    }
    position my_position;
    if (digitalRead(THANOS_v_METHANOS_SWITCH) == METHANOS) {
        my_position = {METHANOS_START, METHANOS_GAUNTLET, 0}; // i.e. current position is METHANOS_START, next position is METHANOS_GAUNTLET, we're 0 of the way there
    } else {
        my_position = {THANOS_START, THANOS_GAUNTLET, 0}; // i.e. current position is METHANOS_START, next position is METHANOS_GAUNTLET, we're 0 of the way there
    }
    initialise_global_variables(my_position, my_state);
}