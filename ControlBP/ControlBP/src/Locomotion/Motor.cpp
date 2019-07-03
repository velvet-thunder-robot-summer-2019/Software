#include "Locomotion/Motor.h"

/**
 * Sets the specified motor to have the given torque in the given direction
 * Params:  motor - LEFT or RIGHT
 *          direction - FWD or BACK
 *          torque - value in N-cm
 */
void run_motor(int motor, int direction, float torque)
{

}

/**
 * Private method for internal use only
 * Takes desired torque as input, outputs estimated PWM needed
 * Params:  torque - wanted torque, N-cm
 * Output:  PWM needed to get wanted torque
 */
float torque_to_PWM(float torque)
{
    return 0.0;
}