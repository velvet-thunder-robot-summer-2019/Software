/** This module translates expected arm angles into movements, and applies PID control to 
 *      correct position
 */

#include "ArmController/ArmDriver.h"

/** Move the arm to the position specified by the angles
 *  @param base_arm_angle: angle of the base arm clockwise from the z-axis in degrees
 *  @param forearm_angle: angle of the forearm clockwise from the direction of the base arm in degrees.
 *  @param wrist_angle: angle of the wrist clockwise from the direction of the forearm in degrees
 *  @param turntable_angle: The turning angle of the turntable from the x axis in degrees.
 *  Returns: MOVE_SUCCESS if the required configuration is reached
 *           MOVE_FAILED if the required configuration cannot be obtained
 */
byte move_whole_arm_position(float base_arm_angle, float forearm_angle, float wrist_angle, float turntable_angle)
{
    return MOVE_SUCCESS;
}

/** Move the turntable to the new position
 *  @param delta_turntable_angle: difference between expected base arm angle and actual potentiometer value
 *  Returns: MOVE_SUCCESS if the required configuration is reached
 *           MOVE_FAILED if the required configuration cannot be obtained
 */
byte move_turntable_position(float delta_turntable_angle)
{
    uint16_t duration = calculate_turntable_movement_time(delta_turntable_angle);

    return MOVE_SUCCESS;
}

/** Move the base arm to the new position
 *  @param delta_base_arm_angle: difference between expected base arm angle and actual potentiometer value
 *  Returns: MOVE_SUCCESS if the required configuration is reached
 *           MOVE_FAILED if the required configuration cannot be obtained
 */
byte move_base_arm_position(float delta_base_arm_angle)
{
    uint16_t duration = calculate_base_arm_movement_time(delta_base_arm_angle);

    return MOVE_SUCCESS;
}

/** Move the forearm to the new position
 *  @param delta_forearm_angle: difference between expected forearm angle and actual potentiometer value
 *  Returns: MOVE_SUCCESS if the required configuration is reached
 *           MOVE_FAILED if the required configuration cannot be obtained
 */
byte move_forearm_position(float delta_forearm_angle)
{
    return MOVE_SUCCESS;
}

/** Move the wrist to the new position
 *  @param delta_wrist_angle: difference between expected wrist angle and actual potentiometer value
 *  Returns: MOVE_SUCCESS if the required configuration is reached
 *           MOVE_FAILED if the required configuration cannot be obtained
 */
byte move_wrist_position(float delta_wrist_angle)
{
    return MOVE_SUCCESS;
}

/** Calculate an estimated duration needed to move the turntable to the new position
 *  @param delta_turntable_angle: difference between expected turntable angle and actual potentiometer value
 *  Returns: movement time in ms
 */
uint16_t calculate_turntable_movement_time(float delta_turntable_angle)
{
    return 0;
}

/** Calculate an estimated duration needed to move the base arm to the new position
 *  @param delta_base_arm_angle: difference between expected base arm angle and actual potentiometer value
 *  Returns: movement time in ms
 */
uint16_t calculate_base_arm_movement_time(float delta_base_arm_angle)
{
    return 0;
}

/** Calculate an estimated duration needed to move the forearm to the new position
 *  @param delta_forearm_angle: difference between expected forearm angle and actual potentiometer value
 *  Returns: movement time in ms
 */
uint16_t calculate_forearm_movement_time(float delta_forearm_angle)
{
    return 0;
}

/** Calculate an estimated duration needed to move the wrist to the new position
 *  @param delta_wrist_angle: difference between expected wrist angle and actual potentiometer value
 *  Returns: movement time in ms
 */
uint16_t calculate_wrist_movement_time(float delta_wrist_angle)
{
    return 0;
}



