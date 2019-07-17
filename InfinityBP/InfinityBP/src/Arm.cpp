#include <Arduino.h>
#include "Arm.h"
#include "CommandExecution.h"

/**
 * Gives the angle of the lazy susan, shoulder, elbow, wrist
 * Params:  arm_positions - pointer in which to place the resulting ang
 * Returns: COMM_SUCCESS if successul
 *          COMM_TASK_FAILED if something goes horribly woefully wrong
 */
byte get_arm_position(byte *arm_positions)
{
    Serial.print("getting arm position");
    return COMM_SUCCESS;
}

/**
 * Sets arm to best position for travel
 * Returns: COMM_SUCCESS if successul
 *          COMM_TASK_FAILED if something goes wrong
 */
byte set_travel_position()
{
    Serial.print("setting arm position to travel");
    return COMM_SUCCESS;
}

/**
 * Sets arm to best position for travel
 * Returns: COMM_SUCCESS if successul
 *          COMM_TASK_FAILED if something goes wrong
 */
byte set_ascent_position()
{
    Serial.print("setting arm position to climb hill");
    return COMM_SUCCESS;
}

/**
 * Places stone into gauntlet (moves from gripping the stone, turns to line up with next gauntlet hole etc)
 * Returns: COMM_SUCCESS if successul
 *          COMM_TASK_FAILED if something goes wrong
 */
byte set_stone_in_gauntlet()
{
    Serial.print("placing stone in gauntlet");
    return COMM_SUCCESS;
}

/**
 * Confirm that post is aligned to bot
 * Returns: COMM_SUCCESS if post is there
 *          COMM_TASK_FAILED otherwise
 */
byte confirm_post_presence()
{
    Serial.print("confirming post presence");
    return COMM_SUCCESS;
}

/**
 * Moves arm up above post, until it clicks the little switch below
 * Returns: COMM_SUCCESS if successul
 *          COMM_TASK_FAILED if something goes wrong (i.e. switch doesn't get clicked)
 */
byte ascend_post()
{
    Serial.print("ascending post");
    return COMM_SUCCESS;
}

/**
 * Closes claw to grab stone
 * Returns: COMM_SUCCESS if successul
 *          COMM_TASK_FAILED if something goes wrong
 */
byte grab_stone()
{
    Serial.print("grabbed stone");
    return COMM_SUCCESS;
}