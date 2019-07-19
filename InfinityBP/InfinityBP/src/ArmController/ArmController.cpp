/** This module translates high-level positioning instructions into a series of arm angle adjustments.
 *      These angle adjustments are then passed into an arm driver, which executes the corrections
 */

#include "ArmController/ArmDriver.h"
#include "ArmController/ArmController.h"
#include "ArmController/AngleCalculator.h"
#include "ArmController/ArmSensors.h"
#include "GauntletController/GauntletController.h"
#include "CommandExecution.h"
#include "HardwareDefs.h"

/** Set up the initial position of the arm, the value of the gauntlet positions, and the post heights
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte init_arm(void)
{
    init_arm_sensors();
    init_arm_driver();

    //Initialize the gauntlet positions in mm
    gauntlet_positions[0].x = GAUNTLET_POS_1_X;
    gauntlet_positions[0].y = GAUNTLET_POS_1_Y;
    gauntlet_positions[0].z = GAUNTLET_POS_1_Z;

    gauntlet_positions[1].x = GAUNTLET_POS_2_X;
    gauntlet_positions[1].y = GAUNTLET_POS_2_Y;
    gauntlet_positions[1].z = GAUNTLET_POS_2_Z;

    gauntlet_positions[2].x = GAUNTLET_POS_3_X;
    gauntlet_positions[2].y = GAUNTLET_POS_3_Y;
    gauntlet_positions[2].z = GAUNTLET_POS_3_Z;

    gauntlet_positions[3].x = GAUNTLET_POS_4_X;
    gauntlet_positions[3].y = GAUNTLET_POS_4_Y;
    gauntlet_positions[3].z = GAUNTLET_POS_4_Z;

    //Initialize the post heights
    post_heights[0] = POST_1_HEIGHT;
    post_heights[1] = POST_2_HEIGHT;
    post_heights[2] = POST_3_HEIGHT;
    post_heights[3] = POST_4_HEIGHT;
    post_heights[4] = POST_5_HEIGHT;
    post_heights[5] = POST_6_HEIGHT;

    //Initialize the starting position of the arm
    turntable_angle = TURNTABLE_TRAVEL_ANGLE;
    base_arm_angle = BASE_ARM_TRAVEL_ANGLE;
    forearm_angle = FORE_ARM_TRAVEL_ANGLE;
    wrist_angle = WRIST_TRAVEL_ANGLE;

    //Open the claw
    open_claw();

    arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
    if (arm_move_status == MOVE_FAIL)
    {
        return COMM_TASK_FAILED;
    }

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

    return COMM_SUCCESS;
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

    arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
    if (arm_move_status == MOVE_FAIL)
    {
        return COMM_TASK_FAILED;
    }

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

    arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
    if (arm_move_status == MOVE_FAIL)
    {
        return COMM_TASK_FAILED;
    }

    return COMM_SUCCESS;
}

/** Unfolds and swings the arm along one side of the robot, until a post is detected.
 *  Postcondition: The arm is centred on the post
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

    arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
    if (arm_move_status == MOVE_FAIL)
    {
        return COMM_TASK_FAILED;
    }

    //slowly move the arm across a small arc to determine the exact center of the post
    starting_turntable_angle = turntable_angle;

    while (starting_turntable_angle - turntable_angle < TURNTABLE_SEARCH_ARC)
    {

        //TODO: Read a tape sensor for a proximity value;
        current_proximity_value =;

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
            arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle + 360);
        }
        else
        {
            arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
        }

        if (arm_move_status == MOVE_FAIL)
        {
            return COMM_TASK_FAILED;
        }
    }

    //Go to the position with the minimum proximity value; this is the arm position perpendicular to the post
    arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, minimum_proximity_angle);
    if (arm_move_status == MOVE_FAIL)
    {
        return COMM_TASK_FAILED;
    }

    return COMM_SUCCESS;
}

/** Climbs the post until the top is reached. 
 *  Precondition: the arm is currently centred on the post
 *  @param post_number: the number of the post to climb
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte ascend_post_to_top(byte post_number)
{

    int16_t current_proximity_value;

    if ( (turntable_angle == UNREACHABLE_ERROR) || (base_arm_angle == UNREACHABLE_ERROR) || \
        (forearm_angle == UNREACHABLE_ERROR) || (wrist_angle == UNREACHABLE_ERROR) )
    {
        return COMM_TASK_FAILED;
    }

    x = calculate_xpos(turntable_angle, base_arm_angle, forearm_angle);
    y = calculate_ypos(turntable_angle, base_arm_angle, forearm_angle);
    xy = calculate_xy_projection(x, y);
    z = post_heights[post_number];

    //Make the initial movement to a position close to the top of the arm
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

    arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
    if (arm_move_status == MOVE_FAIL)
    {
        return COMM_TASK_FAILED;
    }

    //Slowly creep up to the top of the arm to eliminate any positioning errors
    current_proximity_value = 0;
    while (read_tape)
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

        arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
        if (arm_move_status == MOVE_FAIL)
        {
            return COMM_TASK_FAILED;
        }

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
    
    open_claw();

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

    arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
    if (arm_move_status == MOVE_FAIL)
    {
        return COMM_TASK_FAILED;
    }

    close_claw();

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

    //wait until the arm has actually moved over to the correct position
    arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
    if (arm_move_status == MOVE_FAIL)
    {
        return COMM_TASK_FAILED;
    }

    return COMM_SUCCESS;
}

/** Move arm towards the appropriate slot, reposition wrist angle, opens claw
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte put_stone_in_gauntlet(void)
{
    float temp_angle;

    x = gauntlet_positions[current_slot].x;
    y = gauntlet_positions[current_slot].y;
    z = gauntlet_positions[current_slot].z;
    xy = calculate_xy_projection(x, y);

    temp_angle = calculate_arm_angle(xy, z);
    if (temp_angle == UNREACHABLE_ERROR)
    {
        return COMM_TASK_FAILED;
    } 
    else
    {
        base_arm_angle = temp_angle;
    }
    
    temp_angle = calculate_forearm_angle(xy, z);
    if (temp_angle == UNREACHABLE_ERROR)
    {
        return COMM_TASK_FAILED;
    }
    else
    {
        forearm_angle = temp_angle;
    }
    
    wrist_angle = calculate_wrist_angle(base_arm_angle, forearm_angle);

    //wait until the whole arm has moved over to the correct position
    arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
    if (arm_move_status == MOVE_FAIL)
    {
        return COMM_TASK_FAILED;
    }

    open_claw();

    return COMM_SUCCESS;
}

/** Maintain the arm at its current position
 */
void maintain_current_arm_position(void)
{

    //Calculate the relative differences between stated and current position
    float delta_turntable_angle = read_turntable_angle() - turntable_angle;
    float delta_base_arm_angle = read_base_arm_angle() - base_arm_angle;
    float delta_forearm_angle = read_base_arm_angle() - forearm_angle;

    pwm_response base_arm_correction = calculate_base_arm_pwm(delta_base_arm_angle);
    pwm_response forearm_correction = calculate_forearm_pwm(delta_forearm_angle);
    pwm_response turntable_correction = calculate_turntable_pwm(delta_turntable_angle);

    if (base_arm_correction.dir == CLOCKWISE)
    {
        pwm_start(BASE_ARM_POS_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, base_arm_correction.pwm_val * PWM_PERIOD, 0);
        pwm_start(BASE_ARM_NEG_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 0);
    }
    else
    {
        pwm_start(BASE_ARM_POS_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 0);
        pwm_start(BASE_ARM_NEG_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, base_arm_correction.pwm_val * PWM_PERIOD, 0);            
    }

    if (forearm_correction.dir == CLOCKWISE)
    {
        pwm_start(FORE_ARM_POS_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, forearm_correction.pwm_val * PWM_PERIOD, 0);
        pwm_start(FORE_ARM_NEG_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 0);  
    }
    else
    {
        pwm_start(FORE_ARM_POS_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 0);
        pwm_start(FORE_ARM_NEG_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, forearm_correction.pwm_val * PWM_PERIOD, 0);            
    }

    if (turntable_correction.dir == CLOCKWISE)
    {
        pwm_start(TURNTABLE_POS_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, turntable_correction.pwm_val * PWM_PERIOD, 0);
        pwm_start(TURNTABLE_NEG_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 0);  
    }
    else
    {
        pwm_start(TURNTABLE_POS_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 0);
        pwm_start(TURNTABLE_NEG_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, turntable_correction.pwm_val * PWM_PERIOD, 0);            
    }

    wrist_servo.write(wrist_angle);
    
}
