#ifndef ARM_H_
#define ARM_H_

/**
 * Gives the angle of the lazy susan, shoulder, elbow, wrist
 * Params:  arm_positions - pointer in which to place the resulting ang
 * Returns: COMM_SUCCESS if successul
 *          COMM_TASK_FAILED if something goes horribly woefully wrong
 */
byte get_arm_position(byte *arm_positions);

/**
 * Sets arm to best position for travel
 * Returns: COMM_SUCCESS if successul
 *          COMM_TASK_FAILED if something goes wrong
 */
byte set_travel_position();

/**
 * Sets arm to best position for travel
 * Returns: COMM_SUCCESS if successul
 *          COMM_TASK_FAILED if something goes wrong
 */
byte set_ascent_position();

/**
 * Places stone into gauntlet (moves from gripping the stone, turns to line up with next gauntlet hole etc)
 * Returns: COMM_SUCCESS if successul
 *          COMM_TASK_FAILED if something goes wrong
 */
byte set_stone_in_gauntlet();

/**
 * Confirm that post is aligned to bot
 * Returns: COMM_SUCCESS if post is there
 *          COMM_TASK_FAILED otherwise
 */
byte confirm_post_presence();

/**
 * Moves arm up above post, until it clicks the little switch below
 * Returns: COMM_SUCCESS if successul
 *          COMM_TASK_FAILED if something goes wrong (i.e. switch doesn't get clicked)
 */
byte ascend_post();

/**
 * Closes claw to grab stone
 * Returns: COMM_SUCCESS if successul
 *          COMM_TASK_FAILED if something goes wrong
 */
byte grab_stone();

#endif // ARM_H_