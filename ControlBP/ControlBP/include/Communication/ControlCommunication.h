#ifndef CONTROL_COMMUNICATION_H_
#define CONTROL_COMMUNICATION_H_

#include <Arduino.h>

/**
 * Doesn't wait for response bc we can prob just GO without return 
 * Requests for Infinity BP to position arm for travel
 */
int request_arm_position__travel();

/**
 * Requests for Infinity BP to position arm for ascent
 * Returns: Error code returned by Infinity BP, code TBD
 */
int request_arm_position__ascent();

/**
 * Checks whether or not a U-turn has been performed yet (start of course)
 * Returns: True if U turn has been performed
 *          False otherwise
 */
int request_U_turn_status();

/**
 * CONSIDER RETURNING POINTER
 */
int request_recent_path();
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
int request_confirmation_post_presence(int side);

/**
 * Requests ascent of post (kind of climb up post, abstract this)
 * Internally checks if it's at the top
 * Returns:     SUCCESS - if it detects top of post
 *              ERROR   - if something goes horribly wrong
 */
int request_post_ascent();

/**
 * Requests grab infinity stone
 * See doc
 */
int grab_infinity_stone();

/**
 * Request put stone in gauntlet
 */
int request_put_stone_in_gauntlet(int next_slot_available);

/**
 * Request angle of lazy susan, ccw from standard axis
 * Params:      angle - in degrees
 * Returns:     angle set
 */
int request_susan_angle(int angle);


/**
 * Request angle of shoulder, from horizontal axis
 * Params:      angle - in degrees
 * Returns:     angle set
 */
int request_shoulder_angle(int angle);

/**
 * Request angle of elbow 
 * -90 is collinear to  base arm, +90 is extended opposite to base arm
 * Params:      angle - in degrees
 * Returns:     angle set
 */
int request_elbow_angle(int angle);

/**
 * Request angle of wrist 
 * -90 is collinear to mid-arm, +90 is extended opposite to mid-arm
 * Params:      angle - in degrees
 * Returns:     angle set
 */
int request_wrist_angle(int angle);

/**
 * Opens or closes claw
 * Params:      position - OPEN or CLOSED
 * Returns:     claw angle
 */
int request_claw_position(int position);



#endif // CONTROL_COMMUNICATION_H_