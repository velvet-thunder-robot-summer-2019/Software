/** This module translates high-level positioning instructions into a series of arm angle adjustments.
 *      These angle adjustments are then passed into an arm driver, which executes the corrections
 */

#include "ArmController/ArmDriver.h"
#include "ArmController/ArmController.h"
#include "ArmController/AngleCalculator.h"
#include "ArmController/ArmSensors.h"
#include "GauntletController/GauntletController.h"
#include "Communications/CommandExecution.h"
#include "GlobalInfo/HardwareDefs.h"
#include "GlobalInfo/GlobalVars.h"
#include <Arduino.h>

float x, y, z, xy;

uint8_t current_slot;
uint8_t arm_move_status;
uint8_t gauntlet_move_status;

float turntable_angle;
float base_arm_angle;
float forearm_angle;
float wrist_angle;

float temp_angle_1;

coordinate post_positions_left[6];   //location of the stone, relative to the arm base on the stop position
coordinate post_positions_right[6];
coordinate gauntlet_positions[4];
coordinate adaptive_response_left_2;
coordinate adaptive_response_right_3;
coordinate adaptive_response;

uint8_t next_post;

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

    //initialize the position of the posts relative to the arm base,
    //  if approached from the left, in mm
    post_positions_left[0].x = POST_LEFT_1_X;
    post_positions_left[0].y = POST_LEFT_1_Y;
    post_positions_left[0].z = POST_LEFT_1_Z;

    post_positions_left[1].x = POST_LEFT_2_X;
    post_positions_left[1].y = POST_LEFT_2_Y;
    post_positions_left[1].z = POST_LEFT_2_Z;

    post_positions_left[2].x = POST_LEFT_3_X;
    post_positions_left[2].y = POST_LEFT_3_Y;
    post_positions_left[2].z = POST_LEFT_3_Z;

    post_positions_left[3].x = POST_LEFT_4_X;
    post_positions_left[3].y = POST_LEFT_4_Y;
    post_positions_left[3].z = POST_LEFT_4_Z;

    post_positions_left[4].x = POST_LEFT_5_X;
    post_positions_left[4].y = POST_LEFT_5_Y;
    post_positions_left[4].z = POST_LEFT_5_Z;

    post_positions_left[5].x = POST_LEFT_6_X;
    post_positions_left[5].y = POST_LEFT_6_Y;
    post_positions_left[5].z = POST_LEFT_6_Z;

    //Initialize the position of the posts relative to the arm base,
    // if approached from the right, in mm
    post_positions_right[0].x = POST_RIGHT_1_X;
    post_positions_right[0].y = POST_RIGHT_1_Y;
    post_positions_right[0].z = POST_RIGHT_1_Z;

    post_positions_right[1].x = POST_RIGHT_2_X;
    post_positions_right[1].y = POST_RIGHT_2_Y;
    post_positions_right[1].z = POST_RIGHT_2_Z;

    post_positions_right[2].x = POST_RIGHT_3_X;
    post_positions_right[2].y = POST_RIGHT_3_Y;
    post_positions_right[2].z = POST_RIGHT_3_Z;

    post_positions_right[3].x = POST_RIGHT_4_X;
    post_positions_right[3].y = POST_RIGHT_4_Y;
    post_positions_right[3].z = POST_RIGHT_4_Z;

    post_positions_right[4].x = POST_RIGHT_5_X;
    post_positions_right[4].y = POST_RIGHT_5_Y;
    post_positions_right[4].z = POST_RIGHT_5_Z;

    post_positions_right[5].x = POST_RIGHT_6_X;
    post_positions_right[5].y = POST_RIGHT_6_Y;
    post_positions_right[5].z = POST_RIGHT_6_Z;

    //Initialize the adaptive response bit
    adaptive_response_left_2.x = LEFT_2_X;
    adaptive_response_left_2.x = LEFT_2_X;
    adaptive_response_left_2.x = LEFT_2_X;

    adaptive_response_right_3.x = RIGHT_3_X;
    adaptive_response_right_3.x = RIGHT_3_X;
    adaptive_response_right_3.x = RIGHT_3_X;

    #if !MOCK_HARDWARE
        //Open the claw
        open_claw();

        //initialize the starting position of the arm
        position_arm_for_travel();
    #endif

    #if DEBUG_ALL

        Serial.println("Arm Controller initialized");
    
    #endif

    return COMM_SUCCESS;
}

/** Populates an array with the currently held arm angles
 *  @param response[]: A pointer to an array
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte get_arm_position(byte response[])
{
    response[0] = turntable_angle;
    response[1] = base_arm_angle;
    response[2] = forearm_angle;
    response[3] = wrist_angle;

    return COMM_SUCCESS;
}

/** Folds the arm back into its ready position.
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte position_arm_for_travel(void)
{

    #if !MOCK_HARDWARE
        
        turntable_angle = TURNTABLE_TRAVEL_ANGLE;
        base_arm_angle = BASE_ARM_TRAVEL_ANGLE;
        forearm_angle = FORE_ARM_TRAVEL_ANGLE;
        wrist_angle = WRIST_TRAVEL_ANGLE;

        arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
        if (arm_move_status == MOVE_FAIL)
        {
            return COMM_TASK_FAILED;
        }

    #endif

    #if DEBUG_ALL
        Serial.println("arm positioned for travel");
    #endif

    return COMM_SUCCESS;
}

/** Grab a stone from a pillar
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte obtain_infinity_stone(byte side, byte post_number)
{

    #if !MOCK_HARDWARE
    
        //lower the gauntlet to an open position to derestrict movement
        #if DEBUG_ALL
            Serial.println("lowering gauntlet");
        #endif

        gauntlet_move_status = gauntlet_open_position();
        if (gauntlet_move_status != COMM_SUCCESS)
        {
            return COMM_TASK_FAILED;
        }

        //move turntable and arm to a start position on the correct side

        #if DEBUG_ALL
            Serial.println("moving turntable and arm to search position: ");
        #endif

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

        //move turntable to the position of the pillar
        #if DEBUG_ALL
            Serial.println("moving turntable to face pillar");
        #endif

        if (side == LEFT_SIDE)
        {
            temp_angle_1 = calculate_turntable_angle(post_positions_left[post_number - 1].x, post_positions_left[post_number - 1].y);
        }
        else
        {
            temp_angle_1 = calculate_turntable_angle(post_positions_right[post_number - 1].x, post_positions_right[post_number - 1].y);   
        }

        if (temp_angle_1 == UNREACHABLE_ERROR)
        {
            return COMM_TASK_FAILED;
        }
        turntable_angle = temp_angle_1;

        arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
        if (arm_move_status == MOVE_FAIL)
        {
            return COMM_TASK_FAILED;
        }

        //lower the arm to meet the pillar and enclose the stone
        #if DEBUG_ALL
            Serial.println("lowering arm to top of pillar");
        #endif

        if (side == LEFT_SIDE)
        {
            xy = calculate_xy_projection(post_positions_left[post_number - 1].x, post_positions_left[post_number - 1].y);

            temp_angle_1 = calculate_arm_angle(xy, post_positions_left[post_number - 1].z);
            if (temp_angle_1 == UNREACHABLE_ERROR)
            {
                return COMM_TASK_FAILED;
            }
            base_arm_angle = temp_angle_1;

            temp_angle_1 = calculate_forearm_angle(xy, post_positions_left[post_number - 1].z);
            if (temp_angle_1 == UNREACHABLE_ERROR)
            {
                return COMM_TASK_FAILED;
            }
            forearm_angle = temp_angle_1;

            temp_angle_1 = calculate_wrist_angle(base_arm_angle, forearm_angle);
            if (temp_angle_1 == UNREACHABLE_ERROR)
            {
                return COMM_TASK_FAILED;
            }
            wrist_angle = temp_angle_1;
        }
        else
        {
            xy = calculate_xy_projection(post_positions_right[post_number - 1].x, post_positions_right[post_number - 1].y);

            temp_angle_1 = calculate_arm_angle(xy, post_positions_right[post_number - 1].z);
            if (temp_angle_1 == UNREACHABLE_ERROR)
            {
                return COMM_TASK_FAILED;
            }
            base_arm_angle = temp_angle_1;

            temp_angle_1 = calculate_forearm_angle(xy, post_positions_right[post_number - 1].z);
            if (temp_angle_1 == UNREACHABLE_ERROR)
            {
                return COMM_TASK_FAILED;
            }
            forearm_angle = temp_angle_1;

            temp_angle_1 = calculate_wrist_angle(base_arm_angle, forearm_angle);
            if (temp_angle_1 == UNREACHABLE_ERROR)
            {
                return COMM_TASK_FAILED;
            }
            wrist_angle = temp_angle_1;
        }

        arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
        if (arm_move_status == MOVE_FAIL)
        {
            return COMM_TASK_FAILED;
        }

        //grab the stone.
        #if DEBUG_ALL
            Serial.println("closing claw");
        #endif
        close_claw();

        //lift the stone above the pillar again
        #if DEBUG_ALL
            Serial.println("lift stone above pillar");
        #endif

        base_arm_angle = BASE_ARM_SEARCH_ANGLE;
        forearm_angle = FORE_ARM_SEARCH_ANGLE;
        wrist_angle = WRIST_SEARCH_ANGLE;

        arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
        if (arm_move_status == MOVE_FAIL)
        {
            return COMM_TASK_FAILED;
        }

        #if ADAPTIVE_RESPONSE
        
            //Commence adaptive response for short pillars
            if ( (side == LEFT_SIDE && post_number == 3) || (side == RIGHT_SIDE && post_number == 2) )
            {
            
                if (side == LEFT_SIDE && post_number == 3)
                {
                    next_post = 2;
                    adaptive_response = adaptive_response_left_2;
                }
                else if (side == RIGHT_SIDE && post_number == 2)
                {
                    next_post = 3;
                    adaptive_response = adaptive_response_right_3;
                }
            
                //put current stone in gauntlet
                put_stone_in_gauntlet();

                //move turntable to the position of the pillar
                #if DEBUG_ALL
                    Serial.println("moving turntable to face pillar");
                #endif

                temp_angle_1 = calculate_turntable_angle(adaptive_response.x, adaptive_response.y);   

                if (temp_angle_1 == UNREACHABLE_ERROR)
                {
                    return COMM_TASK_FAILED;
                }
                turntable_angle = temp_angle_1;

                arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
                if (arm_move_status == MOVE_FAIL)
                {
                    return COMM_TASK_FAILED;
                }

                //lower the arm to meet the pillar and enclose the stone
                #if DEBUG_ALL
                    Serial.println("lowering arm to top of pillar");
                #endif

                xy = calculate_xy_projection(adaptive_response.x, adaptive_response.y);

                temp_angle_1 = calculate_arm_angle(xy, adaptive_response.z);
                if (temp_angle_1 == UNREACHABLE_ERROR)
                {
                    return COMM_TASK_FAILED;
                }
                base_arm_angle = temp_angle_1;

                temp_angle_1 = calculate_forearm_angle(xy, adaptive_response.z);
                if (temp_angle_1 == UNREACHABLE_ERROR)
                {
                    return COMM_TASK_FAILED;
                }
                forearm_angle = temp_angle_1;

                temp_angle_1 = calculate_wrist_angle(base_arm_angle, forearm_angle);
                if (temp_angle_1 == UNREACHABLE_ERROR)
                {
                    return COMM_TASK_FAILED;
                }
                wrist_angle = temp_angle_1;
                

                arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
                if (arm_move_status == MOVE_FAIL)
                {
                    return COMM_TASK_FAILED;
                }

                //grab the stone.
                #if DEBUG_ALL
                    Serial.println("closing claw");
                #endif
                close_claw();

                //lift the stone above the pillar again
                #if DEBUG_ALL
                    Serial.println("lift stone above pillar");
                #endif
            }

        #endif
        

        //return to travel position
        #if DEBUG_ALL
            Serial.println("returning to travel position");
        #endif
        turntable_angle = TURNTABLE_TRAVEL_ANGLE;
        base_arm_angle = BASE_ARM_TRAVEL_ANGLE;
        forearm_angle = FORE_ARM_TRAVEL_ANGLE;
        wrist_angle = WRIST_TRAVEL_ANGLE;

        arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
        if (arm_move_status == MOVE_FAIL)
        {
            return COMM_TASK_FAILED;
        }

    #endif

    return COMM_SUCCESS;
}

/** [DEPRECATED] 
 * Unfolds and swings the arm along one side of the robot, until a post is detected.
 *  Postcondition: The arm is centred on the post
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte find_post(byte side, byte post_number)
{   

    #if !MOCK_HARDWARE
        
        uint16_t starting_turntable_angle;

        uint16_t current_proximity_value;
        uint16_t minimum_proximity_value = 1023;
        int16_t minimum_proximity_angle = 0;

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

        while ( abs(starting_turntable_angle - turntable_angle) < TURNTABLE_SEARCH_ARC)
        {

            current_proximity_value = read_tape_sensor_analog();

            if (current_proximity_value > minimum_proximity_value)
            {
                minimum_proximity_value = current_proximity_value;
                minimum_proximity_angle = turntable_angle;
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

    #endif

    #if DEBUG_ALL
        Serial.println("stone has been retrieved");
    #endif

    return COMM_SUCCESS;
}

/** [DEPRECATED] 
 * Climbs the post until the top is reached. 
 *  Precondition: the arm is currently centred on the post
 *  @param post_number: the number of the post to climb
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte ascend_post_to_top(byte post_number)
{

    #if !MOCK_HARDWARE

        //int16_t current_proximity_value;
        float temp_angle;

        if ( (turntable_angle == UNREACHABLE_ERROR) || (base_arm_angle == UNREACHABLE_ERROR) || \
            (forearm_angle == UNREACHABLE_ERROR) || (wrist_angle == UNREACHABLE_ERROR) )
        {
            return COMM_TASK_FAILED;
        }

        x = calculate_xpos(turntable_angle, base_arm_angle, forearm_angle);
        y = calculate_ypos(turntable_angle, base_arm_angle, forearm_angle);
        xy = calculate_xy_projection(x, y);
        z = post_positions_right[post_number - 1].z;

        //Make the initial movement to a position close to the top of the arm
        temp_angle = calculate_arm_angle(xy, z);
        if (temp_angle == UNREACHABLE_ERROR)
        {
            return COMM_TASK_FAILED;
        }
        base_arm_angle = temp_angle;
        
        temp_angle = calculate_forearm_angle(xy, z);
        if (temp_angle == UNREACHABLE_ERROR)
        {
            return COMM_TASK_FAILED;
        }
        forearm_angle = temp_angle;

        temp_angle = calculate_wrist_angle(base_arm_angle, forearm_angle);
        if (temp_angle == UNREACHABLE_ERROR)
        {
            return COMM_TASK_FAILED;
        }
        wrist_angle = temp_angle;

        arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
        if (arm_move_status == MOVE_FAIL)
        {
            return COMM_TASK_FAILED;
        }

        //Slowly creep up to the top of the arm to eliminate any positioning errors
        //current_proximity_value = 0;
        while (read_tape_sensor_digital())
        {
            z = z + VERTICAL_STEP_RESOLUTION;

            temp_angle = calculate_arm_angle(xy, z);
            if (temp_angle == UNREACHABLE_ERROR)
            {
                return COMM_TASK_FAILED;
            }
            base_arm_angle = temp_angle;

            temp_angle = calculate_forearm_angle(xy, z);
            if (temp_angle == UNREACHABLE_ERROR)
            {
                return COMM_TASK_FAILED;
            }
            forearm_angle = temp_angle;

            temp_angle = calculate_wrist_angle(base_arm_angle, forearm_angle);
            if (temp_angle == UNREACHABLE_ERROR)
            {
                return COMM_TASK_FAILED;
            }
            wrist_angle = temp_angle;

            arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
            if (arm_move_status == MOVE_FAIL)
            {
                return COMM_TASK_FAILED;
            }

            //current_proximity_value = 0;
        }

    #endif

    return COMM_SUCCESS;
}

/** [DEPRECATED] 
 * Opens the claw, extends the arm towards the post, closes the claw, raises arm clear.
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte grab_infinity_stone(void)
{

    #if !MOCK_HARDWARE
        
        float x_extension, y_extension;
        float temp_angle;
        
        open_claw();

        //extend the arm towards the post
        x_extension = calculate_x_extension(x, y);
        y_extension = calculate_y_extension(x, y);
        x = x + x_extension;
        y = y + y_extension;
        xy = calculate_xy_projection(x, y);

        temp_angle = calculate_arm_angle(xy, z);
        if (temp_angle == UNREACHABLE_ERROR)
        {
            return COMM_TASK_FAILED;
        }
        base_arm_angle = temp_angle;

        temp_angle = calculate_forearm_angle(xy, z);
        if (temp_angle == UNREACHABLE_ERROR)
        {
            return COMM_TASK_FAILED;
        }
        forearm_angle = temp_angle;

        temp_angle = calculate_wrist_angle(base_arm_angle, forearm_angle);
        if (temp_angle == UNREACHABLE_ERROR)
        {
            return COMM_TASK_FAILED;
        }
        wrist_angle = temp_angle;

        arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
        if (arm_move_status == MOVE_FAIL)
        {
            return COMM_TASK_FAILED;
        }

        close_claw();

        //Move the claw up to a safe distance above the post
        z = z + Z_PULLUP_DISTANCE;

        temp_angle = calculate_arm_angle(xy, z);
        if (temp_angle == UNREACHABLE_ERROR)
        {
            return COMM_TASK_FAILED;
        }
        base_arm_angle = temp_angle;

        temp_angle = calculate_forearm_angle(xy, z);
        if (temp_angle == UNREACHABLE_ERROR)
        {
            return COMM_TASK_FAILED;
        }
        forearm_angle = temp_angle;

        temp_angle = calculate_wrist_angle(base_arm_angle, forearm_angle);
        if (temp_angle == UNREACHABLE_ERROR)
        {
            return COMM_TASK_FAILED;
        }
        wrist_angle = temp_angle;

        //wait until the arm has actually moved over to the correct position
        arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
        if (arm_move_status == MOVE_FAIL)
        {
            return COMM_TASK_FAILED;
        }

    #endif

    #if DEBUG_ALL

        Serial.println("infinity stone obtained.");

    #endif

    return COMM_SUCCESS;
}

/** Move arm towards the appropriate slot, reposition wrist angle, opens claw
 *  Returns: COMM_SUCCESS if the operation is successful
 *           COMM_TASK_FAILED if the operation encounters a problem
 */
byte put_stone_in_gauntlet(void)
{

    #if !MOCK_HARDWARE
        
        //move turntable to the appropriate position in front of the gauntlet
        #if DEBUG_ALL
            Serial.println("move turntable to gauntlet");
        #endif

        temp_angle_1 = calculate_turntable_angle(gauntlet_positions[current_slot].x, gauntlet_positions[current_slot].y);
        if (temp_angle_1 == UNREACHABLE_ERROR)
        {
            return COMM_TASK_FAILED;
        }
        turntable_angle = temp_angle_1;

        arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
        if (arm_move_status == MOVE_FAIL)
        {
            return COMM_TASK_FAILED;
        }

        //lower arm to the gauntlet position
        #if DEBUG_ALL
            Serial.println("lower arm to gauntlet");
        #endif

        xy = calculate_xy_projection(gauntlet_positions[current_slot].x, gauntlet_positions[current_slot].y);
        
        temp_angle_1 = calculate_arm_angle(xy, gauntlet_positions[current_slot].z);
        if (temp_angle_1 == UNREACHABLE_ERROR)
        {
            return COMM_TASK_FAILED;
        }
        base_arm_angle = temp_angle_1;

        temp_angle_1 = calculate_forearm_angle(xy, gauntlet_positions[current_slot].z);
        if (temp_angle_1 == UNREACHABLE_ERROR)
        {
            return COMM_TASK_FAILED;
        }
        forearm_angle = temp_angle_1;

        temp_angle_1 = calculate_wrist_angle(xy, gauntlet_positions[current_slot].z);
        if (temp_angle_1 == UNREACHABLE_ERROR)
        {
            return COMM_TASK_FAILED;
        }
        wrist_angle = temp_angle_1;

        arm_move_status = move_whole_arm_position(base_arm_angle, forearm_angle, wrist_angle, turntable_angle);
        if (arm_move_status == MOVE_FAIL)
        {
            return COMM_TASK_FAILED;
        }

        //open claw to drop the stone into the gauntlet
        #if DEBUG_ALL
            Serial.println("dropping stone");
        #endif

        open_claw();

        //increment the number of the occupied slots
        current_slot++;


    #endif

    #if DEBUG_ALL

        Serial.println("stone inserted into gauntlet");

    #endif

    return COMM_SUCCESS;
}

/** Maintain the arm at its current position
 */
void maintain_current_arm_position(void)
{

    #if !MOCK_HARDWARE
        
        //Calculate the relative differences between stated and current position
        #if ACTIVATE_TURNTABLE
            float delta_turntable_angle = read_turntable_angle() - turntable_angle;
            pwm_response turntable_correction = calculate_turntable_pwm(delta_turntable_angle);
        #endif
        #if ACTIVATE_BASE_ARM
            float delta_base_arm_angle = base_arm_angle - read_base_arm_angle();
            pwm_response base_arm_correction = calculate_base_arm_pwm(delta_base_arm_angle);
        #endif

        #if ACTIVATE_BASE_ARM
            if (base_arm_correction.dir == CLOCKWISE)
            {
                pwm_start(BASE_ARM_CW_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, (BASE_ARM_DUTY_CYCLE + base_arm_correction.pwm_val) * PWM_PERIOD, 0);
                pwm_start(BASE_ARM_CCW_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 0);
            }
            else
            {
                pwm_start(BASE_ARM_CW_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 0);
                pwm_start(BASE_ARM_CCW_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, (BASE_ARM_DUTY_CYCLE + base_arm_correction.pwm_val) * PWM_PERIOD, 0);            
            }
        #endif

        #if ACTIVATE_TURNTABLE        
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
        #endif

    #endif

    #if DEBUG_ALL

        //Serial.println("maintaining current position");

    #endif

}
