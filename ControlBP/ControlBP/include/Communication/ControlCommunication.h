#ifndef CONTROL_COMMUNICATION_H_
#define CONTROL_COMMUNICATION_H_

#define COMM_SUCCESS 0
#define COMM_TIMEOUT 1
#define COMM_CORRUPT_RESPONSE 2

#include <Arduino.h>

/**
 * Initialises inter BP communication
 */
void init_communication();


/**
 * Confirm that communication is functional
 */
uint8_t establish_communication();

/**
 * Doesn't wait for response bc we can prob just GO without return 
 * Requests for Infinity BP to position arm for travel
 */
uint8_t request_arm_position__travel();

/**
 * Requests for Infinity BP to position arm for ascent
 * Returns: Error code returned by Infinity BP, code TBD
 */
uint8_t request_arm_position__ascent();

/**
 * 
 */
uint8_t request_confirmation_post_presence(uint8_t side, state expected_state);


/**
 * Gives the angle of all joints in the following order:
 * 1. lazy susan (360, but must manually scale)
 * 2. Shoulder (0 to 180)
 * 3. Elbow (0 to 180)
 * 4. Wrist (0 to 180)
 */
uint8_t get_arm_angles(uint8_t *angles);

/**
 * Requests ascent of post (kind of climb up post, abstract this)
 * Internally checks if it's at the top
 * Returns:     SUCCESS - if it detects top of post
 *              ERROR   - if something goes horribly wrong
 */
uint8_t request_post_ascent();

/**
 * Requests grab infinity stone
 * See doc
 */
uint8_t grab_infinity_stone();

/**
 * Request put stone in gauntlet
 */
uint8_t request_put_stone_in_gauntlet();

// /**
//  * Request angle of lazy susan, ccw from standard axis
//  * Params:      angle - in degrees
//  * Returns:     angle set
//  */
// int request_susan_angle(int angle);


// /**
//  * Request angle of shoulder, from horizontal axis
//  * Params:      angle - in degrees
//  * Returns:     angle set
//  */
// int request_shoulder_angle(int angle);

// /**
//  * Request angle of elbow 
//  * -90 is collinear to  base arm, +90 is extended opposite to base arm
//  * Params:      angle - in degrees
//  * Returns:     angle set
//  */
// int request_elbow_angle(int angle);

// /**
//  * Request angle of wrist 
//  * -90 is collinear to mid-arm, +90 is extended opposite to mid-arm
//  * Params:      angle - in degrees
//  * Returns:     angle set
//  */
// int request_wrist_angle(int angle);

// /**
//  * Opens or closes claw
//  * Params:      position - OPEN or CLOSED
//  * Returns:     claw angle
//  */
// int request_claw_position(int position);



#endif // CONTROL_COMMUNICATION_H_