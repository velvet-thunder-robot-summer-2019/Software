#ifndef LOCOMOTION_H_
#define LOCOMOTION_H_

/**
 * Performs necessary initialisation for tape following
 * Initialises PID and tape-sensing. Must be called before follow_tape
 */
void init_tape_following();

/**
 * Adjusts PWM of both motors once
 * Returns:     SUCCESS if successful
 *              TAPE_NOT_FOUND if tape repeatedly fails to be found
 */
int follow_tape(float torque);

/**
 * move backwards in straight line
 */
int reverse(float pwm);

/**
 * Stops motors
 */
int stop_motors();

/**
 * Stops motors too
 */
int stop_motors(int current_direction);


/**
 * Turns the robot along circle of arc length rho (cm), at speed prop to pwm,
 * to turn in direction given
 */
int follow_arc_rho(int direction, int rho, float smaller_pwm);

/**
 * Aligns robot to "branch" in the road, i.e. to tape leading to post
 * Makes sure both wing sensors on the expected side are on the tape
 * Params:      side - side on which branch / post is expected
 * Returns:     SUCCESS - if alignment is successfully accomplished
 *              STATE_CHANGED - if state is changed during backtracking
 */
int align_to_branch();

/**
 * Turns the robot to face ALONG THE TAPE, in the direction of the home gauntlet
 * Calls on decision making module to decide which way to go
 * Checks identity as Thanos or Methanos to pick direction
 * Returns:        SUCCESS - if tape successfully found
 *                  STATE_CHANGED - if state changed during task
 */
int align_direction_to_return();

/**
 * Aligns bot to Gauntlet. May only involve following the black tape tbh
 * Returns:        SUCCESS - if tape successfully found
 *                 STATE_CHANGED - if state changed during task
 */
int align_to_gauntlet();

/**
 * Rotates bot on the spot (clockwise bc I felt like it)
 */
int rotate_on_spot(float pwm, int direction);



#endif // LOCOMOTION_H_