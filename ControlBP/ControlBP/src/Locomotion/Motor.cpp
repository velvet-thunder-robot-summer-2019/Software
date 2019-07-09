#include <Arduino.h>
#include "Locomotion/Motor.h"
#include "GlobalInfo/HardwareDefs.h"

#define LEFT_MOTOR_PWM_FWD PB_9
#define LEFT_MOTOR_PWM_BACK PB_8
#define RIGHT_MOTOR_PWM_FWD PA_6
#define RIGHT_MOTOR_PWM_BACK PA_7

#define CLOCK_FREQUENCY 100000 //Hz
#define PERIOD 500 //T(pwm) = PERIOD/CLOCK_FREQUENCY

float torque_to_PWM(int torque);

uint32_t PWM_initialised = 0;

/**
 * Sets the specified motor to have the given torque in the given direction
 * Params:  motor - LEFT or RIGHT
 *          direction - FWD or BACK
 *          torque - value in N-cm
 * NOTE: just takes in PWM value rn
 */
void run_motor(int motor, int direction, int torque)
{
    PinName fwd;
    PinName back;

    float duty_cycle = torque_to_PWM(torque);

    if (motor == LEFT_MOTOR) {
        fwd = LEFT_MOTOR_PWM_FWD;
        back = LEFT_MOTOR_PWM_BACK;
    } else {
        fwd = RIGHT_MOTOR_PWM_FWD;
        back = RIGHT_MOTOR_PWM_BACK;
    }

    if (direction == FWD) {
        pwm_stop(back);
        pwm_start(fwd, CLOCK_FREQUENCY, PERIOD, duty_cycle * PERIOD, !PWM_initialised);
    } else {
        pwm_stop(fwd);
        pwm_start(back, CLOCK_FREQUENCY, PERIOD, duty_cycle * PERIOD, !PWM_initialised);
    }

    if (!PWM_initialised) {
        PWM_initialised = 1;
    }
}

/**
 * Private method for internal use only
 * Takes desired torque as input, outputs estimated PWM needed
 * Params:  torque - wanted torque, N-cm
 * Output:  PWM needed to get wanted torque
 */
float torque_to_PWM(int torque)
{
    return torque;
}

