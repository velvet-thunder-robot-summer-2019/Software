#ifndef CONTROL_COMMUNICATION_H_
#define CONTROL_COMMUNICATION_H_

#include <Arduino.h>

/**
 * Doesn't wait for response bc we can prob just GO without return 
 * Requests for Infinity BP to position arm for travel
 */
void request_arm_position__travel();

/**
 * Requests for Infinity BP to position arm for ascent
 * Returns: Error code returned by Infinity BP, code TBD
 */
uint8_t request_arm_position__ascent();

/**
 * Checks whether or not a U-turn has been performed yet (start of course)
 * Returns: True if U turn has been performed
 *          False otherwise
 */
uint8_t request_U_turn_status();

/**
 * CONSIDER RETURNING POINTER
 */
uint8_t request_recent_path();
/**
 * Returns:     number of left wheel rotations
 */
int request_wheel_rotations_left();

/**
 * CONSIDER returning a struct with left and right wheel info (2 ints)
 * Returns:     number of right wheel rotations
 */
int request_wheel_rotations_right();

/**
 * 
 */
void request_confirmation_post_presence();



#endif // CONTROL_COMMUNICATION_H_