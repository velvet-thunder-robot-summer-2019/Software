/** This module translates expected arm angles into movements, and applies PID control to 
 *      correct position
 */

/** Move the arm to the position specified by the angles
 *  @param theta1: angle of the base arm clockwise from the z-axis in degrees
 *  @param theta2: angle of the forearm clockwise from the direction of the base arm in degrees.
 *  @param theta3: angle of the wrist clockwise from the direction of the forearm in degrees
 *  @param phi: The turning angle of the turntable from the x axis in degrees.
 *  Returns: status of operation
 */
int move_whole_arm_position(float theta1, float theta2, float theta3, float phi)
{
    return 0;
}

/** Move the base arm to the new position
 *  @param deltaTheta1: difference between expected base arm angle and actual potentiometer value
 *  Returns: status of operation
 */
int move_base_arm_position(float deltaTheta1)
{
    return 0;
}

/** Move the forearm to the new position
 *  @param deltaTheta2: difference between expected forearm angle and actual potentiometer value
 *  Returns: status of operation
 */
int move_forearm_position(float deltaTheta2)
{
    return 0;
}

/** Move the wrist to the new position
 *  @param deltaTheta3: difference between expected wrist angle and actual potentiometer value
 *  Returns: status of operation
 */
int move_wrist_position(float deltaTheta3)
{
    return 0;
}

/** Calculate an estimated duration needed to move the turntable to the new position
 *  @param deltaPhi: difference between expected turntable angle and actual potentiometer value
 *  Returns: status of operation
 */
int calculate_turntable_movement_time(float deltaPhi)
{
    return 0;
}

/** Calculate an estimated duration needed to move the base arm to the new position
 *  @param deltaTheta1: difference between expected base arm angle and actual potentiometer value
 *  Returns: status of operation
 */
int calculate_base_arm_movement_time(float deltaTheta1)
{
    return 0;
}

/** Calculate an estimated duration needed to move the forearm to the new position
 *  @param deltaTheta2: difference between expected forearm angle and actual potentiometer value
 *  Returns: status of operation
 */
int calculate_forearm_movement_time(float deltaTheta2)
{
    return 0;
}

/** Calculate an estimated duration needed to move the wrist to the new position
 *  @param deltaTheta3: difference between expected wrist angle and actual potentiometer value
 *  Returns: status of operation
 */
int calculate_wrist_movement_time(float deltaTheta3)
{
    return 0;
}



