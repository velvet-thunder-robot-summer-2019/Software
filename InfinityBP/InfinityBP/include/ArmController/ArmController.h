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

#define Z_PULLUP_DISTANCE 0

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

byte get_arm_position(byte response[]);

/** Fold the arm into an ascent position
 *  Returns: status of operation
 */
byte position_arm_for_ascent(void);

/** Folds the arm back into its ready position.
 *  Returns: status of operation
 */
byte position_arm_for_travel(void);

/** Unfolds and swings the arm along one side of the robot, until a post is detected.
 *      The arm is then centred at the post 
 *  Returns: status of operation
 */
byte find_post(byte side);

/** Climbs the post until the top is reached. 
 *  Returns: status of operation
 */
byte ascend_post_to_top(void);

/** Opens the claw, extends the arm towards the post, closes the claw, raises arm clear.
 *  Returns: status of operation
 */
byte grab_infinity_stone(void);

/** Move arm towards the appropriate slot, reposition wrist angle, opens claw
 *  Returns: status of operation
 */
byte put_stone_in_gauntlet(void);