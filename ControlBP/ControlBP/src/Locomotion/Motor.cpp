#include <Arduino.h>

#include "Locomotion/Motor.h"
#include "GlobalInfo/HardwareDefs.h"

#define CLOCK_FREQUENCY 100000 //Hz
#define PERIOD 500 //T(pwm) = PERIOD/CLOCK_FREQUENCY

int motors_initialised = FALSE;

/**
 * Sets the specified motor to have the given torque in the given direction
 * Params:  motor - LEFT or RIGHT
 *          direction - FWD or BACK
 *          torque - value in N-cm
 * NOTE: just takes in PWM value rn
 */
void run_motor(int motor, int direction, float duty_cycle)
{
    if (!motors_initialised) {
        pwm_start(LEFT_MOTOR_PWM_BACK, CLOCK_FREQUENCY, PERIOD, 0, 1);
        pwm_start(LEFT_MOTOR_PWM_FWD, CLOCK_FREQUENCY, PERIOD, 0, 1);
        pwm_start(RIGHT_MOTOR_PWM_BACK, CLOCK_FREQUENCY, PERIOD, 0, 1);
        pwm_start(RIGHT_MOTOR_PWM_FWD, CLOCK_FREQUENCY, PERIOD, 0, 1);

        motors_initialised = TRUE;
    }
    PinName fwd;
    PinName back;

    if (motor == LEFT) {
         duty_cycle *= 0.85;
    }

    if (motor == LEFT_MOTOR) {
        fwd = LEFT_MOTOR_PWM_FWD;
        back = LEFT_MOTOR_PWM_BACK;
    } else {
        fwd = RIGHT_MOTOR_PWM_FWD;
        back = RIGHT_MOTOR_PWM_BACK;
    }

    if (direction == FWD) {
        pwm_start(back, CLOCK_FREQUENCY, PERIOD, 0, 0);
        pwm_start(fwd, CLOCK_FREQUENCY, PERIOD, duty_cycle * PERIOD, 0);
    } else {
        pwm_start(fwd, CLOCK_FREQUENCY, PERIOD, 0, 0);
        pwm_start(back, CLOCK_FREQUENCY, PERIOD, duty_cycle * PERIOD, 0);
    }
}
