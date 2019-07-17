/** This module translates high-level positioning instructions into a series of arm angle adjustments.
 *      These angle adjustments are then passed into an arm driver, which executes the corrections
 */

#include "ArmController/ArmDriver.h"

/** Fold the arm into an ascent position
 *  Returns: status of operation
 */
int position_arm_for_ascent(void)
{
    return 0;
}

/** Folds the arm back into its ready position.
 *  Returns: status of operation
 */
int position_arm_at_ready(void)
{
    return 0;
}

/** Unfolds and swings the arm along one side of the robot, until a post is detected.
 *      The arm is then centred at the post 
 *  Returns: status of operation
 */
int find_post(int side)
{
    return 0;
}

/** Climbs the post until the top is reached. 
 *  Returns: status of operation
 */
int ascend_post_to_top()
{
    return 0;
}

/** Opens the claw, extends the arm towards the post, closes the claw, raises arm clear.
 *  Returns: status of operation
 */
int grab_infinity_stone()
{
    return 0;
}

/** Move arm towards the appropriate slot, reposition wrist angle, opens claw
 *  Returns: status of operation
 */
int put_stone_in_gauntlet(int slot_number)
{
    return 0;
}

/** Maintain the arm at its current position indefinitely until interrupted externally
 *  Returns: status of operation
 */
int maintain_current_arm_position(void)
{
    return 0;
}
