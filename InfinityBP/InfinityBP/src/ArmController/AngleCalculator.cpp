/** This module calculates and returns the angles position the arm at a given xyz coordinate.
 *  The coordinate system used is centred at the joint securing the lower arm to the chassis
 *  The x-axis is directed 90 degrees clockwise from the forward direction of the chassis
 *  The y-axis is directed to the forward direction of the chassis
 *  The z-axis is directed upwards, perpendicular to the x and y axis.
 */

#include "ArmController/AngleCalculator.h"
#include "GlobalInfo/HardwareDefs.h"
#include <math.h>

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
 *          If the specified position is unreachable, returns an error code
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
        return UNREACHABLE_ERROR;
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
        return UNREACHABLE_ERROR;
    }
    
    return theta;
}

/** Calculates the necessary rotational angle of the wrist joint of the forearm, relative to the current direction of
 *      the upper arm, in a clockwise direction.
 * @param armAngle: The angle in degrees of the lower arm, measured clockwise from the z-axis. This angle has values between
 *                      -90 and 270 inclusive
 * @param foreArmAngle: The angle in degrees of the upper arm, measured clockwise from the direction of the lower arm. This angle
 *                      has values between -180 and 180 inclusive
 * Returns: The necessary rotational angle of the wrist joint of the forearm. This angle has values between 0 and 180 inclusive, where
 *                      the wrist at 90 degrees is collinear to the forearm.
 *          If the angle is unreachable, return an error code
 */
float calculate_wrist_angle(float armAngle, float foreArmAngle)
{   
    float theta = 180 - armAngle - foreArmAngle;

    if (theta <= 180 && theta >= 0)
    {
        return theta;
    }
    
    return UNREACHABLE_ERROR;

}

/** Calculates the x position in mm of the tip of the arm from the base of the arm
 *  @param turntableAngle: The angle in degrees of the turntable, measured anticlockwise from the x-axis. This angle has values
 *                              between 0 and 360 inclusive 
 *  @param armAngle: The angle in degrees of the lower arm, measured clockwise from the z-axis. This angle has values between
 *                              -180 and 180 inclusive
 *  @param foreArmAngle: The angle in degrees of the upper arm, measured clockwise from the direction of the lower arm. This angle
 *                              has values between -180 and 180 inclusive
 *  Returns: The x-position in mm of the tip of the arm from the base of the arm
 */
float calculate_xpos(float turntableAngle, float armAngle, float foreArmAngle)
{
    
    float xyPos;

    xyPos = L1*sin(armAngle * DEG_RAD) + L2*cos((90 - armAngle - foreArmAngle) * DEG_RAD) + L3;

    return xyPos * cos(turntableAngle * DEG_RAD);
    
}

/** Calculates the y position in mm of the tip of the arm from the base of the arm
 *  @param turntableAngle: The angle in degrees of the turntable, measured anticlockwise from the x-axis. This angle has values
 *                              between 0 and 360 inclusive 
 *  @param armAngle: The angle in degrees of the lower arm, measured clockwise from the z-axis. This angle has values between
 *                              -180 and 180 inclusive
 *  @param foreArmAngle: The angle in degrees of the upper arm, measured clockwise from the direction of the lower arm. This angle
 *                              has values between -180 and 180 inclusive
 *  Returns: The y-position in mm of the tip of the arm from the base of the arm
 */
float calculate_ypos(float turntableAngle, float armAngle, float foreArmAngle)
{

    float xyPos;

    xyPos = L1*sin(armAngle * DEG_RAD) + L2*cos((90 - armAngle - foreArmAngle) * DEG_RAD) + L3;

    return xyPos * sin(turntableAngle * DEG_RAD);
    
}

/** Calculates the z position in mm of the tip of the arm from the base of the arm
 *  @param armAngle: The angle in degrees of the lower arm, measured clockwise from the z-axis. This angle has values between
 *                              -180 and 180 inclusive
 *  @param foreArmAngle: The angle in degrees of the upper arm, measured clockwise from the direction of the lower arm. This angle
 *                              has values between -180 and 180 inclusive
 *  Returns: The z-position in mm of the tip of the arm from the base of the arm
 */
float calculate_zpos(float armAngle, float foreArmAngle)
{

    float zPos;

    zPos = L1*cos(armAngle * DEG_RAD) + L2*cos((armAngle + foreArmAngle) * DEG_RAD);

    return zPos;
    
}

/** Calculate how much the arm needs to extend in the x direction to grab a stone
 *  @param x: x-position of the tip of the claw in mm
 *  @param y: y-position of the tip of the claw in mm
 *  Returns: the extension length in the x direction in mm
 */
float calculate_x_extension(float x, float y)
{
    return EXTENSION_LENGTH * x / sqrt(pow(x,2) + pow(y,2));
}

/** Calculate how much the arm needs to extend in the y direction to grab a stone
 *  @param x: x-position of the tip of the claw in mm
 *  @param y: y-position of the tip of the claw in mm
 *  Returns: the extension length in the y direction in mm
 */
float calculate_y_extension(float x, float y)
{
    return EXTENSION_LENGTH * y / sqrt(pow(x,2) + pow(y,2));
}



