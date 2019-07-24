#include <Arduino.h>

#include "AllPurposeInclude.h"

#include "Locomotion/PID.h"
#include "Locomotion/Motor.h"

#define CLICKS_PER_DEGREE 

float PID_output = 0;

/**
 * Performs necessary initialisation for tape following
 * Initialises PID and tape-sensing. Must be called before follow_tape
 */
void init_tape_following()
{
    // Serial.println("init_tape_following");
    init_PID();
}

/**
 * Adjusts PWM of both motors once
 * Params:      torque - determines speed of motion
 * Returns:     SUCCESS if successful
 *              TAPE_NOT_FOUND if tape repeatedly fails to be found
 */
int follow_tape(float torque)
{
#if DEBUG_PRINT
    Serial.println("follow_tape");
    Serial.print("PWM: ");
    Serial.println(torque);
    Serial.println("");

    Serial.print("PID_output: ");
    Serial.println(PID_output);
#endif
    run_motor(RIGHT_MOTOR, FWD, torque + PID_output);
    run_motor(LEFT_MOTOR, FWD, torque - PID_output);
    int error = get_tape_following_error();
#if DEBUG_PRINT
    Serial.print("tape following error is: ");
    Serial.println(error);
#endif
    PID_output = get_PID_output(error);

    // delay(500);

    return SUCCESS;
}

/**
 * Rotates bot on the spot (clockwise bc I felt like it)
 */
int rotate_on_spot(float pwm)
{
    run_motor(RIGHT_MOTOR, FWD, pwm);
    run_motor(LEFT_MOTOR, BACK, pwm);
    get_tape_following_error();
    
    return SUCCESS;
}

/**
 * Turns the robot along circle of arc length rho (cm), at speed prop to pwm,
 * to turn in direction given
 */
int follow_arc_rho(int direction, int rho, float smaller_pwm)
{
    float larger_pwm = (rho + 0.5 * 11.5) / (rho - 0.5 * 11.5) * smaller_pwm;
    if (direction == RIGHT) {
        run_motor(RIGHT_MOTOR, FWD, smaller_pwm);
        run_motor(LEFT_MOTOR, FWD, larger_pwm);
    } else {
        run_motor(LEFT_MOTOR, FWD, smaller_pwm);
        run_motor(RIGHT_MOTOR, FWD, larger_pwm);
    }
    get_tape_following_error();
    return SUCCESS;
}



/**
 * backs up the robot
 */
int reverse(float pwm)
{
    run_motor(RIGHT_MOTOR, BACK, pwm);
    run_motor(LEFT_MOTOR, BACK, pwm);
    get_tape_following_error();
    return SUCCESS;
}

/**
 * Stops the robot's motors
 */
int stop_motors()
{
    run_motor(RIGHT_MOTOR, BACK, 0.2);
    run_motor(LEFT_MOTOR, BACK, 0.2);
    uint32_t start_time = millis();
    while (millis() - start_time > 2) {
        get_tape_following_error();
    }
    run_motor(RIGHT_MOTOR, FWD, 0);
    run_motor(LEFT_MOTOR, FWD, 0);
    return SUCCESS;
}

/**
 * Backtracks to reach tape, using historical information of movement
 * Checks for any change of state during backtracking
 * Returns:     SUCCESS - if tape successfully found 
 *              STATE_CHANGED - if state is changed during backtracking
 */
int backtrack_to_tape()
{
    Serial.println("backtrack_to_tape");
    digitalWrite(BLINKY, HIGH);
    return SUCCESS;
}

/**
 * Aligns robot to "branch" in the road, i.e. to tape leading to post
 * Makes sure both wing sensors on the expected side are on the tape
 * Params:      side - side on which branch / post is expected
 * Returns:     SUCCESS - if alignment is successfully accomplished
 *              STATE_CHANGED - if state is changed during backtracking
 */
int align_to_branch()
{
    Serial.println("align_to_branch");
    return SUCCESS;
}

/**
 * Turns the robot to face ALONG THE TAPE, in the direction of the home gauntlet
 * Calls on decision making module to decide which way to go
 * Checks identity as Thanos or Methanos to pick direction
 * Returns:        SUCCESS - if tape successfully found
 *                  STATE_CHANGED - if state changed during task
 */
int align_direction_to_return()
{
    Serial.println("align_direction_to_return");
    return SUCCESS;
}

/**
 * Aligns bot to Gauntlet. May only involve following the black tape tbh
 * Returns:        SUCCESS - if tape successfully found
 *                 STATE_CHANGED - if state changed during task
 */
int align_to_gauntlet()
{
    Serial.println("align_to_gauntlet");
    return SUCCESS;
}
