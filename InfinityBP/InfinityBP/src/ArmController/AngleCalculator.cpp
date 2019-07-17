/** This module calculates and returns the angles position the arm at a given xyz coordinate.
 *  The coordinate system used is centred at the joint securing the lower arm to the chassis
 *  The x-axis is directed 90 degrees clockwise from the forward direction of the chassis
 *  The y-axis is directed to the forward direction of the chassis
 *  The z-axis is directed upwards, perpendicular to the x and y axis.
 */

#include "ArmController/AngleCalculator.h"

/** Calculates the absolute length of the extended arm, projected onto the xy plane
 * @param x: The x-coordinate of the arm in mm
 * @param y: The y-coordinate of the arm in mm
 * Returns: The xy projection of the arm in mm
 */
float calculate_xy_projection(float x, float y)
{
    return sqrt(pow(x,2) + pow(y,2));
}

/** Calculates the necessary rotational angle of the turntable normal to the xy plane
 * @param x: The x-coordinate of the tip of the arm in mm
 * @param y: The y-coordinate of the tip of the arm in mm
 * Returns: The turning angle from the x axis in degrees. The turning angle is always a 
 *          value between 0 and 360 inclusive.
 */
float calculate_turntable_angle(float x, float y)
{

    float theta;

    theta = atan(y/x) * RAD_DEG;
    
    if (x < 0)
    {
        theta = theta + 180;
    }
    else if (x > 0 && y < 0)
    {
        theta = theta + 360;
    }

    return theta;
}

/** Calculates the necessary rotational angle of the lowest joint of the arm, relative to the z-axis,
 *      in a clockwise direction
 * @param xy: The xy projection of the arm in mm
 * @param z: The z-coordinate of the tip of the arm in mm
 * Returns: If the specified position is reachable, return the necessary angle clockwise from the z-axis.
 *              The angle is in degrees, having possible values between -180 to 180 inclusive.
 *          If the specified position is unreachable, return an error code.
 */
float calculate_arm_angle(float xy, float z)
{
    float theta1;
    float theta2;
    float fraction2 = (pow(L1, 2) + pow(xy - L3, 2) + pow(z,2) - pow(L2, 2))/(2*L1*sqrt(pow(xy - L3, 2) + pow(z,2)));

    if (fraction2 < 1 && fraction2 > -1)
    {
        theta1 = atan((xy - L3)/z) * RAD_DEG;
        theta2 = acos(fraction2) * RAD_DEG;

        if (z < 0)
        {
            theta1 = theta1 + 180;
        }

        return theta1 - theta2;

    }
    else
    {
        //TODO: Return an error code.
        return 1000;
    }

}

/** Calculates the necessary rotational angle of the second lowest joint of the forearm, relative to the current direction of
 *      the lower arm, in a clockwise direction
 * @param xy: The xy projection of the arm in mm
 * @param z: The z-coordinate of the tip of the arm in mm
 * Returns: If the specified position is reachable, return the necessary angle clockwise from the z-axis
 *          If the specified position is unreachable, return an error code.
 */
float calculate_forearm_angle(float xy, float z)
{

    float theta;
    float fraction = (pow(L1,2) + pow(L2,2) - pow(xy - L3, 2) - pow(z,2))/(2*L1*L2);

    if (fraction < 1 && fraction > -1)
    {
        theta = 180 - acos(fraction) * RAD_DEG;
    }
    else
    {
        //TODO: Return an error code
        theta = 1000;
    }
    
    return theta;
}

/** Calculates the necessary rotational angle of the wrist joint of the forearm, relative to the current direction of
 *      the upper arm, in a clockwise direction.
 * @param armAngle: The angle in degrees of the lower arm, measured clockwise from the z-axis. This angle has values between
 *                      -180 and 180 inclusive
 * @param foreArmAngle: The angle in degrees of the upper arm, measured clockwise from the direction of the lower arm. This angle
 *                      has values between -180 and 180 inclusive
 * Returns: The necessary rotational angle of the wrist joint of the forearm. This angle has values between -180 and 180 inclusive
 */
float calculate_wrist_angle(float armAngle, float foreArmAngle)
{   
    return 90 - armAngle - foreArmAngle;
}
