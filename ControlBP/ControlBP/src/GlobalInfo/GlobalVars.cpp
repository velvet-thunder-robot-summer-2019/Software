#include "GlobalInfo/GlobalVars.h"
#include "GlobalInfo/HardwareDefs.h"
#include <Arduino.h>

volatile status_summary run_status;

/**
 * Gets called from MENU mode for testing
 * Params: start_position - location at which robot is beginning
 *         start_state    - state in which the robot is beginning
 */
void initialise_competition_data(position start_position)
{
    Serial.println("initialise_competition_data");

    // enable fault light
    pinMode(BLINKY, OUTPUT);    

    run_status.bot_position.last_location = start_position.last_location;
    run_status.bot_position.next_location = start_position.next_location;
    int i;
    for (i = 0; i < TOTAL_NUM_STONE; i++) {
        run_status.stones_status[i] = UNKNOWN;
    }

    if (digitalRead(THANOS_v_METHANOS_SWITCH) == METHANOS) {
#if DEBUG_PRINT
        Serial.println("we are methanos");
#endif
        run_status.bot_identity = METHANOS;
    } else {
#if DEBUG_PRINT
        Serial.println("we are inevitable");
#endif
        run_status.bot_identity = THANOS;
    }

    run_status.target_branch = UPPER;
}

/**
 * Initialisation from competition mode
 * Sets up my state as REACH_RAMP, my position as either METHANOS_START or THANOS_START depending on MvT
 */
void initialise_competition_data()
{
#if DEBUG_PRINT
    Serial.println("initialise_competition_data general mode");
    Serial.println("switch says initialised: ");
#endif
    position my_position;
    if (digitalRead(THANOS_v_METHANOS_SWITCH) == METHANOS) {
        Serial.println("we are methanos");
        my_position = {METHANOS_START, METHANOS_GAUNTLET};  // i.e. current position is METHANOS_START, next position is METHANOS_GAUNTLET, we're 0 of the way there
        run_status.bot_identity = METHANOS;
    } else {
        Serial.println("we are thanos");
        my_position = {THANOS_START, THANOS_GAUNTLET}; // i.e. current position is METHANOS_START, next position is METHANOS_GAUNTLET, we're 0 of the way there
        run_status.bot_identity = THANOS;
    }
    initialise_competition_data(my_position);
}