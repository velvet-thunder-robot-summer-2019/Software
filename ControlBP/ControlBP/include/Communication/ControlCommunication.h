#ifndef CONTROL_COMMUNICATION_H_
#define CONTROL_COMMUNICATION_H_

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
<<<<<<< HEAD
=======
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
>>>>>>> ee6590875df00a0e2a3b1ec15e38ceaa455f683b
 * Gives the angle of all joints in the following order:
 * 1. lazy susan (360, but must manually scale)
 * 2. Shoulder (0 to 180)
 * 3. Elbow (0 to 180)
 * 4. Wrist (0 to 180)
 */
uint8_t get_arm_angles(uint8_t *angles);

/**
 * Requests grab infinity stone
 * See doc
 */
uint8_t grab_infinity_stone(byte side, byte post_num);

/**
 * Request put stone in gauntlet
 */
uint8_t request_put_stone_in_gauntlet();

uint8_t send_command_no_retries(byte command); // for debug

uint8_t deploy_gauntlet();

#endif // CONTROL_COMMUNICATION_H_