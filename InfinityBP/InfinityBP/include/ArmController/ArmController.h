#ifndef ARM_CONTROLLER_H_
#define ARM_CONTROLLER_H_

#include <Arduino.h>

/** This module translates high-level positioning instructions into a series of arm angle adjustments.
 *      These angle adjustments are then passed into an arm driver, which executes the corrections
 */

extern float turntable_angle;
extern float base_arm_angle;
extern float forearm_angle;
extern float wrist_angle;

/** Set up the initial position of the arm, the value of the gauntlet positions, and the post heights
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte init_arm(void);

/** Populates an array with the currently held arm angles
 *  @param response[]: A pointer to an array
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte get_arm_position(byte response[]);

/** Fold the arm into an ascent position
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte position_arm_for_ascent(void);

/** Folds the arm back into its ready position.
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte position_arm_for_travel(void);

/** Unfolds and swings the arm along one side of the robot, until a post is detected.
 *  Postcondition: The arm is centred on the post
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte find_post(byte side);

/** Climbs the post until the top is reached. 
 *  Precondition: the arm is currently centred on the post
 *  @param post_number: the number of the post to climb
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte ascend_post_to_top(byte post_number);

/** Opens the claw, extends the arm towards the post, closes the claw, raises arm clear.
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte grab_infinity_stone(void);

/** Move arm towards the appropriate slot, reposition wrist angle, opens claw
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte put_stone_in_gauntlet(void);

/** Maintain the arm at its current position
 */
void maintain_current_arm_position(void);

#endif
