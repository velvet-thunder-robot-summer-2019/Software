/** This module translates high-level positioning instructions into a series of arm angle adjustments.
 *      These angle adjustments are then passed into an arm driver, which executes the corrections
 */

#include <Arduino.h>

#define TURNTABLE_TRAVEL_ANGLE 0
#define BASE_ARM_TRAVEL_ANGLE 0
#define FORE_ARM_TRAVEL_ANGLE 0
#define WRIST_TRAVEL_ANGLE 0

#define TURNTABLE_ASCENT_ANGLE 0
#define BASE_ARM_ASCENT_ANGLE 0
#define FORE_ARM_ASCENT_ANGLE 0
#define WRIST_ASCENT_ANGLE 0

#define TURNTABLE_SEARCH_LEFT 0 
#define TURNTABLE_SEARCH_RIGHT 0
#define BASE_ARM_SEARCH_ANGLE 0
#define FORE_ARM_SEARCH_ANGLE 0
#define WRIST_SEARCH_ANGLE 0

#define TURNTABLE_SEARCH_ARC 0
#define TURNTABLE_STEP_RESOLUTION 0

#define LEFT_SIDE  0
#define RIGHT_SIDE 1

#define POST_1_HEIGHT 0
#define POST_2_HEIGHT 0
#define POST_3_HEIGHT 0
#define POST_4_HEIGHT 0
#define POST_5_HEIGHT 0
#define POST_6_HEIGHT 0

#define GAUNTLET_POS_1_X 0
#define GAUNTLET_POS_1_Y 0
#define GAUNTLET_POS_1_Z 0

#define GAUNTLET_POS_2_X 0
#define GAUNTLET_POS_2_Y 0
#define GAUNTLET_POS_2_Z 0

#define GAUNTLET_POS_3_X 0
#define GAUNTLET_POS_3_Y 0
#define GAUNTLET_POS_3_Z 0

#define GAUNTLET_POS_4_X 0
#define GAUNTLET_POS_4_Y 0
#define GAUNTLET_POS_4_Z 0

#define GAUNTLET_POS_5_X 0
#define GAUNTLET_POS_5_Y 0
#define GAUNTLET_POS_5_Z 0

#define GAUNTLET_POS_6_X 0
#define GAUNTLET_POS_6_Y 0
#define GAUNTLET_POS_6_Z 0

#define Z_PULLUP_DISTANCE 0

#define VERTICAL_STEP_RESOLUTION 0

float turntable_angle;
float base_arm_angle;
float forearm_angle;
float wrist_angle;

float x, y, z, xy;

struct coordinate 
{
    float x;
    float y;
    float z;
}gauntlet_positions[4];

uint8_t current_slot;
uint8_t arm_move_status;

uint16_t post_heights[6];

/** Set up the initial position of the arm, the value of the gauntlet positions, and the post heights
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte init_arm(void)

/** Populates an array with the currently held arm angles
 *  @param response[]: A pointer to an array
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte get_arm_position(byte response[])

/** Fold the arm into an ascent position
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte position_arm_for_ascent(void)

/** Folds the arm back into its ready position.
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte position_arm_for_travel(void)

/** Unfolds and swings the arm along one side of the robot, until a post is detected.
 *  Postcondition: The arm is centred on the post
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte find_post(byte side)

/** Climbs the post until the top is reached. 
 *  Precondition: the arm is currently centred on the post
 *  @param post_number: the number of the post to climb
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte ascend_post_to_top(byte post_number)

/** Opens the claw, extends the arm towards the post, closes the claw, raises arm clear.
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte grab_infinity_stone(void)

/** Move arm towards the appropriate slot, reposition wrist angle, opens claw
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte put_stone_in_gauntlet(void)

/** Maintain the arm at its current position
 */
void maintain_current_arm_position(void)