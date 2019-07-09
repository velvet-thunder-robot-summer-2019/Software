#include <Arduino.h>

#include "GlobalInfo/HardwareDefs.h"
#include "Locomotion/TapeSensor.h"


#define DEBUG_BRANCH_REACH_EXPECTED 20

int branch_reach_calls = 0;
int tape_sensor_threshold = 400;

/**
 * Returns the error in tape following from 2 front tape sensor input
 * (ON_TAPE, LEFT_OFF_RIGHT_ON, 
 * RIGHT_OFF_LEFT_ON, BOTH_OFF_LAST_RIGHT,
 * BOTH_OFF_LAST_LEFT)
 */
int get_tape_following_error()
{
    Serial.println("get_tape_following_error");
    return 0;
}

/**
 * Returns true if branch has been reached, false otherwise (as detected by wing sensors)
 * Params:  expected_side - side on which we expect a branch to appear
 */
int branch_reached(int expected_side) {
    Serial.print("branch_reached, expected side: ");
    Serial.println(expected_side);
    if (branch_reach_calls < DEBUG_BRANCH_REACH_EXPECTED) {
        branch_reach_calls++;
        return 0;
    }
    return 1;
}

/**
 * Updates cutoff threshold for tape sensors to potentiometer value
 */
void update_threshold_tape_sensor()
{
    Serial.println("update_threshold_tape_sensor");
    tape_sensor_threshold = analogRead(CALIBRATION_POTENTIOMETER);
}

/**
 * Returns the current threshold for tape sensor values
 */
int get_tape_sensor_threshold() {
    return tape_sensor_threshold;
}