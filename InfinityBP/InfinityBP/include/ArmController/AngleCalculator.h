/** This module calculates and returns the angles position the arm at a given xyz coordinate.
 *  The coordinate system used is centred at the joint securing the lower arm to the chassis
 *  The x-axis is directed 90 degrees clockwise from the forward direction of the chassis
 *  The y-axis is directed to the forward direction of the chassis
 *  The z-axis is directed upwards, perpendicular to the x and y axis.
 */

#include "Arduino.h"
#include <math.h>

#define L1 20
#define L2 20
#define L3 5
#define RAD_DEG 180.0/3.1415926535
#define DEG_RAD 3.1415925635/180.0

/** Calculates the absolute length of the extended arm, projected onto the xy plane
 * @param x: The x-coordinate of the arm in mm
 * @param y: The y-coordinate of the arm in mm
 * Returns: The xy projection of the arm in mm
 */
float calculate_xy_projection(float x, float y);

/** Calculates the necessary rotational angle of the turntable normal to the xy plane
 * @param x: The x-coordinate of the tip of the arm in mm
 * @param y: The y-coordinate of the tip of the arm in mm
 * Returns: The turning angle from the x axis in degrees. The turning angle is always a 
 *          value between 0 and 360 inclusive.
 */
float calculate_arm_angle(float xy, float z);

/** Calculates the necessary rotational angle of the lowest joint of the arm, relative to the z-axis,
 *      in a clockwise direction
 * @param xy: The xy projection of the arm in mm
 * @param z: The z-coordinate of the tip of the arm in mm
 * Returns: If the specified position is reachable, return the necessary angle clockwise from the z-axis.
 *              The angle is in degrees, having possible values between -180 to 180 inclusive.
 *          If the specified position is unreachable, return an error code.
 */
float calculate_turntable_angle(float x, float y);

/** Calculates the necessary rotational angle of the second lowest joint of the forearm, relative to the current direction of
 *      the lower arm, in a clockwise direction
 * @param xy: The xy projection of the arm in mm
 * @param z: The z-coordinate of the tip of the arm in mm
 * Returns: If the specified position is reachable, return the necessary angle clockwise from the z-axis
 *          If the specified position is unreachable, return an error code.
 */
float calculate_forearm_angle(float xy, float z);

/** Calculates the necessary rotational angle of the wrist joint of the forearm, relative to the current direction of
 *      the upper arm, in a clockwise direction.
 * @param armAngle: The angle in degrees of the lower arm, measured clockwise from the z-axis. This angle has values between
 *                      -180 and 180 inclusive
 * @param foreArmAngle: The angle in degrees of the upper arm, measured clockwise from the direction of the lower arm. This angle
 *                      has values between -180 and 180 inclusive
 * Returns: The necessary rotational angle of the wrist joint of the forearm. This angle has values between -180 and 180 inclusive
 */
float calculate_wrist_angle(float armAngle, float foreArmAngle);