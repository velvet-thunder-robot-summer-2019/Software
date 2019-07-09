#ifndef TAPE_SENSOR_H_
#define TAPE_SENSOR_H_

/**
 * Returns the error in tape following from 2 front tape sensor input
 * (ON_TAPE, LEFT_OFF_RIGHT_ON, 
 * RIGHT_OFF_LEFT_ON, BOTH_OFF_LAST_RIGHT,
 * BOTH_OFF_LAST_LEFT)
 */
int get_tape_following_error();

/**
 * Returns true if branch has been reached, false otherwise (as detected by wing sensors)
 * Params:  expected_side - side on which we expect a branch to appear
 */
int branch_reached(int expected_side);

/**
 * Updates cutoff threshold for tape sensors to potentiometer value
 */
void update_threshold_tape_sensor();

/**
 * Returns the current threshold for tape sensor values
 */
int get_tape_sensor_threshold();

#endif // TAPE_SENSOR_H_