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
    run_status.bot_position.left_wheel_ticks = start_position.left_wheel_ticks;
    run_status.bot_position.right_wheel_ticks = start_position.right_wheel_ticks;

    int i;
    for (i = 0; i < TOTAL_NUM_STONE; i++) {
        run_status.stones_status[i] = UNKNOWN;
    }

    run_status.last_collision = NO_COLLISION;

    if (digitalRead(THANOS_v_METHANOS_SWITCH) == METHANOS) {
        Serial.println("we are methanos");
        run_status.bot_identity = METHANOS;
    } else {
        Serial.println("we are inevitable");
        run_status.bot_identity = THANOS;
    }
}

/**
 * Initialisation from competition mode
 * Sets up my state as REACH_RAMP, my position as either METHANOS_START or THANOS_START depending on MvT
 */
void initialise_competition_data()
{
    Serial.println("initialise_competition_data general mode");

    position my_position;
    Serial.println("switch says initialised: ");
    if (digitalRead(THANOS_v_METHANOS_SWITCH) == METHANOS) {
        Serial.println("we are methanos");
        my_position = {METHANOS_START, METHANOS_GAUNTLET, 0, 0}; //, 0, 0, 0}; // i.e. current position is METHANOS_START, next position is METHANOS_GAUNTLET, we're 0 of the way there
        run_status.bot_identity = METHANOS;
    } else {
        Serial.println("we are thanos");
        my_position = {THANOS_START, THANOS_GAUNTLET, 0, 0}; //, 0, 0, 0}; // i.e. current position is METHANOS_START, next position is METHANOS_GAUNTLET, we're 0 of the way there
        run_status.bot_identity = THANOS;
    }
    initialise_competition_data(my_position);
}