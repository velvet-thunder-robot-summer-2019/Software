#include <Arduino.h>

#include "AllPurposeInclude.h"

#include "Locomotion/Locomotion.h"
#include "Locomotion/PID.h"
#include "Locomotion/TapeSensor.h"
#include "Locomotion/Motor.h"

float PID_output = 0;

/**
 * Performs necessary initialisation for tape following
 * Initialises PID and tape-sensing. Must be called before follow_tape
 */
void init_tape_following()
{
    Serial.println("init_tape_following");
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
    /*
    Serial.println("follow_tape");
    Serial.print("PWM: ");
    Serial.println(torque);
    Serial.println("");

    Serial.print("PID_output: ");
    Serial.println(PID_output);
    */
    run_motor(RIGHT_MOTOR, FWD, torque + PID_output);
    run_motor(LEFT_MOTOR, BACK, torque - PID_output);
    int error = get_tape_following_error();
    // Serial.print("tape following error is: ");
    // Serial.println(error);
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
