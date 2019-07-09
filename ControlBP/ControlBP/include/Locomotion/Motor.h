#ifndef MOTOR_H_
#define MOTOR_H_

/**
 * Sets the specified motor to have the given torque in the given direction
 * Params:  motor - LEFT or RIGHT
 *          direction - FWD or BACK
 *          torque - value in N-cm
 */
void run_motor(int motor, int direction, int torque);

#endif // MOTOR_H_