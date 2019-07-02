#include "Initialisation\State.h"
#include "Initialisation/HardwareDefs.h"
#include <Arduino.h>

/**
 * Gets called from MENU mode for testing
 * Params: start_position - location at which robot is beginning
 *         start_state    - state in which the robot is beginning
 */
void initialise_competition_data(position start_position)
{
    bot_position.last_location = start_position.last_location;
    bot_position.next_location = start_position.last_location;
    bot_position.fraction_to_next = start_position.fraction_to_next;

    if (digitalRead(THANOS_v_METHANOS_SWITCH) == METHANOS)
    {
        bot_identity = METHANOS;
    } else {
        bot_identity = THANOS;
    }

    bot_previous_state = MENU; //previous state of robot
    int i;
    for (i = 0; i < TOTAL_NUM_STONE; i++) {
        stones_status[i] = UNKNOWN;
    }
}

/**
 * Initialisation from competition mode
 * Sets up my state as REACH_RAMP, my position as either METHANOS_START or THANOS_START depending on MvT
 */
void initialise_competition_data()
{
    position my_position;
    if (digitalRead(THANOS_v_METHANOS_SWITCH) == METHANOS) {
        my_position = {METHANOS_START, METHANOS_GAUNTLET, 0}; // i.e. current position is METHANOS_START, next position is METHANOS_GAUNTLET, we're 0 of the way there
    } else {
        my_position = {THANOS_START, THANOS_GAUNTLET, 0}; // i.e. current position is METHANOS_START, next position is METHANOS_GAUNTLET, we're 0 of the way there
    }
    initialise_competition_data(my_position);
}