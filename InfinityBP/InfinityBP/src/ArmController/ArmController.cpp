/** This module translates high-level positioning instructions into a series of arm angle adjustments.
 *      These angle adjustments are then passed into an arm driver, which executes the corrections
 */

#include "ArmController/ArmDriver.h"
#include "ArmController/ArmController.h"
#include "ArmController/AngleCalculator.h"
#include "CommandExecution.h"

/** Set up the initial position of the arm.
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte init_arm(void)
{

    gauntlet_positions[0].x = 0.0;
    gauntlet_positions[0].y = 0.0;
    gauntlet_positions[0].z = 0.0;

    gauntlet_positions[1].x = 0.0;
    gauntlet_positions[1].y = 0.0;
    gauntlet_positions[1].z = 0.0;

    gauntlet_positions[2].x = 0.0;
    gauntlet_positions[2].y = 0.0;
    gauntlet_positions[2].z = 0.0;

    gauntlet_positions[3].x = 0.0;
    gauntlet_positions[3].y = 0.0;
    gauntlet_positions[3].z = 0.0;

    turntable_angle = TURNTABLE_TRAVEL_ANGLE;
    base_arm_angle = BASE_ARM_TRAVEL_ANGLE;
    forearm_angle = FORE_ARM_TRAVEL_ANGLE;
    wrist_angle = WRIST_TRAVEL_ANGLE;

    move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);

    return COMM_SUCCESS;
}

/** Populates an array with the currently held arm angles
 *  @param response[]: A pointer to an array
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte get_arm_position(byte response[])
{
    response[1] = turntable_angle;
    response[2] = base_arm_angle;
    response[3] = forearm_angle;
    response[4] = wrist_angle;

    return SUCCESS;
}

/** Fold the arm into an ascent position
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte position_arm_for_ascent(void)
{

    turntable_angle = TURNTABLE_ASCENT_ANGLE;
    base_arm_angle = BASE_ARM_ASCENT_ANGLE;
    forearm_angle = FORE_ARM_ASCENT_ANGLE;
    wrist_angle = WRIST_ASCENT_ANGLE;

    move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);

    return COMM_SUCCESS;
}

/** Folds the arm back into its ready position.
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte position_arm_for_travel(void)
{
    turntable_angle = TURNTABLE_TRAVEL_ANGLE;
    base_arm_angle = BASE_ARM_TRAVEL_ANGLE;
    forearm_angle = FORE_ARM_TRAVEL_ANGLE;
    wrist_angle = WRIST_ASCENT_ANGLE;

    move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);

    return COMM_SUCCESS;
}

/** Unfolds and swings the arm along one side of the robot, until a post is detected.
 *      The arm is then centred at the post 
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte find_post(byte side)
{

    uint16_t starting_turntable_angle;

    uint16_t current_proximity_value;
    uint16_t minimum_proximity_value;
    int16_t minimum_proximity_angle;

    //Move the arm to a starting position, depending on the side
    base_arm_angle = BASE_ARM_SEARCH_ANGLE;
    forearm_angle = FORE_ARM_SEARCH_ANGLE;
    wrist_angle = WRIST_SEARCH_ANGLE;

    if (side == LEFT_SIDE)
    {
        turntable_angle = TURNTABLE_SEARCH_LEFT;
    }
    else
    {
        turntable_angle = TURNTABLE_SEARCH_RIGHT;
    }

    move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);

    //slowly move the arm across a small arc to determine the exact center of the post

    starting_turntable_angle = turntable_angle;

    while (starting_turntable_angle - turntable_angle < TURNTABLE_SEARCH_ARC)
    {

        //TODO: Read a tape sensor for a proximity value;
        current_proximity_value = 0;

        if (current_proximity_value > minimum_proximity_value)
        {
            minimum_proximity_value = current_proximity_value;

            if (turntable_angle < 0)
            {
                minimum_proximity_angle = turntable_angle + 360;
            }
            else
            {
                minimum_proximity_angle = turntable_angle;
            }
        }

        turntable_angle = turntable_angle - TURNTABLE_STEP_RESOLUTION;

        if (turntable_angle < 0)
        {
            move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle + 360);
        }
        else
        {
            move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
        }
    }

    //Go to the position with the minimum proximity value; this is the arm position perpendicular to the post
    move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, minimum_proximity_angle);

    return COMM_SUCCESS;
}

/** Climbs the post until the top is reached. 
 *  Precondition: the arm is currently centred on the post
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */

//TODO: Receive position as a parameter
byte ascend_post_to_top(void)
{

    int16_t current_proximity_value;

    if ( (turntable_angle == UNREACHABLE_ERROR) || (base_arm_angle == UNREACHABLE_ERROR) || \
        (forearm_angle == UNREACHABLE_ERROR) || (wrist_angle == UNREACHABLE_ERROR) )
    {
        return COMM_TASK_FAILED;
    }

    x = calculate_xpos(turntable_angle, base_arm_angle, forearm_angle);
    y = calculate_ypos(turntable_angle, base_arm_angle, forearm_angle);
    z = calculate_zpos(base_arm_angle, forearm_angle);
    xy = calculate_xy_projection(x, y);

    //TODO: obtain the new height z

    //Make the initial movement
    base_arm_angle = calculate_arm_angle(xy, z);
    if (base_arm_angle == UNREACHABLE_ERROR)
    {
        return COMM_TASK_FAILED;
    }

    forearm_angle = calculate_forearm_angle(xy, z);
    if (forearm_angle == UNREACHABLE_ERROR)
    {
        return COMM_TASK_FAILED;
    }

    wrist_angle = calculate_wrist_angle(base_arm_angle, forearm_angle);

    move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);

    //Slowly creep up to the top of the arm
    current_proximity_value = 0;
    while (current_proximity_value > PROXIMITY_THRESHOLD)
    {
        z = z + VERTICAL_STEP_RESOLUTION;

        base_arm_angle = calculate_arm_angle(xy, z);
        if (base_arm_angle == UNREACHABLE_ERROR)
        {
            return COMM_TASK_FAILED;
        }

        forearm_angle = calculate_forearm_angle(xy, z);
        if (forearm_angle == UNREACHABLE_ERROR)
        {
            return COMM_TASK_FAILED;
        }

        wrist_angle = calculate_wrist_angle(base_arm_angle, forearm_angle);

        move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);

        current_proximity_value = 0;
    }

    return COMM_SUCCESS;
}

/** Opens the claw, extends the arm towards the post, closes the claw, raises arm clear.
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte grab_infinity_stone(void)
{
    float x_extension, y_extension;
    //TODO: Open the claw

    //extend the arm towards the post
    x_extension = calculate_x_extension(x, y);
    y_extension = calculate_y_extension(x, y);
    x = x + x_extension;
    y = y + y_extension;
    xy = calculate_xy_projection(x, y);

    base_arm_angle = calculate_arm_angle(xy, z);
    if (base_arm_angle == UNREACHABLE_ERROR)
    {
        return COMM_TASK_FAILED;
    }

    forearm_angle = calculate_forearm_angle(xy, z);
    if (forearm_angle == UNREACHABLE_ERROR)
    {
        return COMM_TASK_FAILED;
    }

    wrist_angle = calculate_wrist_angle(base_arm_angle, forearm_angle);

    move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);

    //TODO: Close the claw

    //Move the claw up to a safe distance above the post
    z = z + Z_PULLUP_DISTANCE;

    base_arm_angle = calculate_arm_angle(xy, z);
    if (base_arm_angle == UNREACHABLE_ERROR)
    {
        return COMM_TASK_FAILED;
    }

    forearm_angle = calculate_forearm_angle(xy, z);
    if (forearm_angle == UNREACHABLE_ERROR)
    {
        return COMM_TASK_FAILED;
    }

    wrist_angle = calculate_wrist_angle(base_arm_angle, forearm_angle);

    move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);

    return COMM_SUCCESS;
}

/** Move arm towards the appropriate slot, reposition wrist angle, opens claw
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte put_stone_in_gauntlet(void)
{
    x = gauntlet_positions[current_slot].x;
    y = gauntlet_positions[current_slot].y;
    z = gauntlet_positions[current_slot].z;
    xy = calculate_xy_projection(x, y);

    base_arm_angle = calculate_arm_angle(xy, z);
    if (base_arm_angle == UNREACHABLE_ERROR)
    {
        return COMM_TASK_FAILED;
    }

    forearm_angle = calculate_forearm_angle(xy, z);
    if (forearm_angle == UNREACHABLE_ERROR)
    {
        return COMM_TASK_FAILED;
    }

    wrist_angle = calculate_wrist_angle(base_arm_angle, forearm_angle);

    move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);

    //TODO: Open the claw

    return COMM_SUCCESS;
}

/** Maintain the arm at its current position indefinitely until interrupted externally
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
void maintain_current_arm_position(void)
{
    //TODO: Execute some PID stuff here, while checking for some conditione
}
