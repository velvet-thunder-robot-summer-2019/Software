#ifndef TAPE_SENSOR_H_
#define TAPE_SENSOR_H_

/**
 * Initialising pins for tape sensing
 */
void initTapeSensor();

/**
 * Returns the error in tape following from 2 front tape sensor input
 * (ON_TAPE, LEFT_OFF_RIGHT_ON, 
 * RIGHT_OFF_LEFT_ON, BOTH_OFF_LAST_RIGHT,
 * BOTH_OFF_LAST_LEFT)
 */
int get_tape_following_error();

// /**
//  * Returns true if branch has been reached, false otherwise (as detected by wing sensors)
//  */
// int branch_reached();

/**
 * Returns TRUE if the front sensors reach a branch, FALSE otherwise
 * Essentially, if a side sensor and ANY other sensor are on tape, we can deduce that 
 * there must be a branch
 */
int branch_reached_front();

/**
 * get tape sensor values from last stop
 */
int last_stop_outer_left();
int last_stop_outer_right();
int last_stop_left();
int last_stop_right();


/**
 * Updates cutoff threshold for tape sensors to potentiometer value
 */
void update_threshold_tape_sensor();

/**
 * Returns the current threshold for tape sensor values
 */
int get_tape_sensor_threshold();

/**
 * Returns: 0 if outer left sensor is over threshold (over white)
 *          1 if outer left sensor is below threshold (over tape)
 */
int outer_left_sensor();

/**
 * Returns: 0 if outer left sensor is over threshold (over white)
 *          1 if outer left sensor is below threshold (over tape)
 */
int outer_right_sensor();

/**
 * Returns: 0 if left sensor is over threshold (over white)
 *          1 if left sensor is below threshold (over tape)
 */
int left_sensor();

/**
 * Returns: 0 if right sensor is over threshold (over white)
 *          1 if right sensor is below threshold (over tape)
 */
int right_sensor();

// /**
//  * Returns:     0 if sensor off
//  *              1 if sensor on
//  */
// int right_wing_sensor();

// /**
//  * Returns:     0 if sensor off
//  *              1 if sensor on
//  */
// int left_wing_sensor();

#endif // TAPE_SENSOR_H_