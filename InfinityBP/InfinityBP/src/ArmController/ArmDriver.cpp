/** This module translates expected arm angles into movements, and applies PID control to 
 *      correct position
 */

#include "ArmController/ArmDriver.h"
#include "ArmController/ArmController.h"
#include "ArmController/ArmSensors.h"
#include "Arduino.h"
#include "Servo.h"

#define NUM_PAST_ERRORS

Servo claw_servo;
Servo wrist_servo;

float turntable_kp = 0.0; 
float base_arm_kp = 0.0;
float forearm_kp = 0.0;

float turntable_ki = 0.0; 
float base_arm_ki = 0.0;
float forearm_ki = 0.0;

float turntable_kd = 0.0; 
float base_arm_kd = 0.0;
float forearm_kd = 0.0;

float turntable_p_response, turntable_i_response, turntable_d_response;
float base_arm_p_response, base_arm_i_response, base_arm_d_response;
float forearm_p_response, forearm_i_response, forearm_d_response;

float turntable_past_errors[NUM_PAST_ERRORS];
float base_arm_past_errors[NUM_PAST_ERRORS];
float forearm_past_errors[NUM_PAST_ERRORS];

float turntable_anti_windup = 0.0;
float base_arm_anti_windup = 0.0;
float forearm_anti_windup = 0.0;

uint8_t turntable_last_error_pos = 0;
uint8_t base_arm_last_error_pos = 0;
uint8_t forearm_last_error_pos = 0;

/** Initializes the arm driver
 */
void init_arm_driver(void)
{
    uin8_t i;

    claw_servo.attach(CLAW_SERVO_PIN);
    claw_servo.write(CLAW_SERVO_OPEN);

    wrist_servo.attach(WRIST_SERVO_PIN);
    wrist_servo.write(WRIST_SERVO_MID);

    pinMode(TURNTABLE_POS_PIN, OUTPUT);
    pinMode(TURNTABLE_NEG_PIN, OUTPUT);

    pinMode(BASE_ARM_POS_PIN, OUTPUT);
    pinMode(BASE_ARM_NEG_PIN, OUTPUT);

    pinMode(FORE_ARM_POS_PIN, OUTPUT);
    pinMode(FORE_ARM_NEG_PIN, OUTPUT);

    pwm_start(BASE_ARM_POS_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 1);
    pwm_start(BASE_ARM_NEG_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 1);
    pwm_start(FORE_ARM_POS_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 1);
    pwm_start(FORE_ARM_NEG_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 1);
    pwm_start(TURNTABLE_POS_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 1);
    pwm_start(TURNTABLE_NEG_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 1);

    for (i = 0; i < NUM_PAST_ERRORS; i++)
    {
        turntable_past_errors[i] = 0;
        base_arm_past_errors[i] = 0;
        forearm_past_errors[i] = 0;
    }

}

/** Move the arm to the position specified by the angles
 *  @param base_arm_angle: angle of the base arm clockwise from the z-axis in degrees
 *  @param forearm_angle: angle of the forearm clockwise from the direction of the base arm in degrees.
 *  @param wrist_angle: angle of the wrist clockwise from the direction of the forearm in degrees
 *  @param turntable_angle: The turning angle of the turntable from the x axis in degrees.
 *  Returns: MOVE_SUCCESS if the required configuration is reached
 *           MOVE_FAILED if the required configuration cannot be obtained
 */
byte move_whole_arm_position(float base_arm_angle, float forearm_angle, float wrist_angle, float turntable_angle)
{

    //Get angle discrepency
    float delta_base_arm_angle = base_arm_angle - read_base_arm_angle();
    float delta_forearm_angle = forearm_angle - read_fore_arm_angle();
    float delta_turntable_angle = turntable_angle - read_turntable_angle();

    pwm_response base_arm_correction; 
    pwm_response forearm_correction;
    pwm_response turntable_correction; 

    while ( (abs(delta_base_arm_angle) > ANGULAR_ERROR_BOUND) && (abs(delta_forearm_angle) > ANGULAR_ERROR_BOUND) \
            && (abs(delta_turntable_angle) > ANGULAR_ERROR_BOUND) )
    {

        base_arm_correction = calculate_base_arm_pwm(delta_base_arm_angle);
        forearm_correction = calculate_forearm_pwm(delta_forearm_angle);
        turntable_correction = calculate_turntable_pwm(delta_turntable_angle);

        if (base_arm_correction.dir == CLOCKWISE)
        {
            pwm_start(BASE_ARM_POS_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, base_arm_correction.pwm_val * PWM_PERIOD, 0);
            pwm_start(BASE_ARM_NEG_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 0);
        }
        else
        {
            pwm_start(BASE_ARM_POS_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 0);
            pwm_start(BASE_ARM_NEG_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, base_arm_correction.pwm_val * PWM_PERIOD, 0);            
        }

        if (forearm_correction.dir == CLOCKWISE)
        {
            pwm_start(FORE_ARM_POS_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, forearm_correction.pwm_val * PWM_PERIOD, 0);
            pwm_start(FORE_ARM_NEG_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 0);  
        }
        else
        {
            pwm_start(FORE_ARM_POS_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 0);
            pwm_start(FORE_ARM_NEG_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, forearm_correction.pwm_val * PWM_PERIOD, 0);            
        }

        if (turntable_correction.dir == CLOCKWISE)
        {
            pwm_start(TURNTABLE_POS_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, turntable_correction.pwm_val * PWM_PERIOD, 0);
            pwm_start(TURNTABLE_NEG_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 0);  
        }
        else
        {
            pwm_start(TURNTABLE_POS_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, 0, 0);
            pwm_start(TURNTABLE_NEG_PIN, PWM_CLOCK_FREQ, PWM_PERIOD, turntable_correction.pwm_val * PWM_PERIOD, 0);            
        }

        delay(100);

    }

    wrist_servo.write(wrist_angle);

    return MOVE_SUCCESS;
}

/** Opens the claw
 */
void open_claw(void)
{
    claw_servo.write(CLAW_SERVO_OPEN);
}

/** Closes the claw
 */
void close_claw(void)
{
    claw_servo.write(CLAW_SERVO_CLOSE);
}

/** Calculate a PID pwm response to the angular difference of the turntable
 *  @param delta_turntable_angle: difference between expected turntable angle and actual potentiometer value
 *  Returns: pwm_response struct
 */
pwm_response calculate_turntable_pwm(float delta_turntable_angle)
{

    //calculate the proportional response
    turntable_p_response = delta_turntable_angle * turntable_kp;

    //calculate the integral response
    turntable_i_response = turntable_i_response + delta_turntable_angle * turntable_ki;
    if (turntable_i_response > turntable_anti_windup)
    {
        turntable_i_response = turntable_anti_windup;
    }
    else if (turntable_i_response < turntable_anti_windup * -1.0)
    {
        turntable_i_response = turntable_anti_windup * -1.0;
    }

    //calculate the derivative response
    turntable_d_response = turntable_kd * (delta_turntable_angle - turntable_past_errors[turntable_last_error_pos]);

    turntable_update_error(delta_turntable_angle);

    return turntable_p_response + turntable_i_response + turntable_d_response;
}

/** Calculate a PID pwm response to the angular difference of the base arm
 *  @param delta_base_arm_angle: difference between expected base arm angle and actual potentiometer value
 *  Returns: pwm_response struct
 */
pwm_response calculate_base_arm_pwm(float delta_base_arm_angle)
{
    //calculate the proprotional response
    base_arm_p_response = delta_base_arm_angle * base_arm_kp;

    //calculate the integral response
    base_arm_i_response = base_arm_i_response + delta_base_arm_angle * base_arm_ki;
    if (base_arm_i_response > base_arm_anti_windup)
    {
        base_arm_i_response = base_arm_anti_windup;
    }
    else if (base_arm_i_response < base_arm_anti_windup * -1.0)
    {
        base_arm_i_response = base_arm_anti_windup * 1.0;
    }

    //calculate the derivative response
    base_arm_d_response = base_arm_kd * (delta_base_arm_angle - base_arm_past_errors[base_arm_last_error_pos]);

    base_arm_update_error(delta_base_arm_angle);

    return base_arm_p_response + base_arm_i_response + base_arm_d_response;
}

/** Calculate a PID pwm response to the angular difference of the forearm
 *  @param delta_forearm_angle: difference between expected forearm angle and actual potentiometer value
 *  Returns: pwm_response_struct
 */
pwm_response calculate_forearm_pwm(float delta_forearm_angle)
{
    
    //calculate the proportional response
    forearm_p_response = delta_forearm_angle * forearm_kp;

    //calculate the integral response
    forearm_i_response = forearm_i_response + delta_forearm_angle * forearm_ki;
    if (forearm_i_response > forearm_anti_windup)
    {
        forearm_i_response = forearm_anti_windup;
    }
    else if (forearm_i_response < forearm_anti_windup * -1.0)
    {
        forearm_i_response = forearm_anti_windup * -1.0;
    }

    //calcualte the derivative response
    forearm_d_response = forearm_kd * ( delta_forearm_angle - forearm_past_errors[forearm_last_error_pos] );

    forearm_update_error(delta_forearm_angle);

    return forearm_p_response + forearm_i_response + forearm_d_response;
}

/** Updates the register of past errors for the turntable angle
 *  @param error: the most recent error
 */
void turntable_update_error(float error)
{
    turntable_past_errors[turntable_last_error_pos] = error;
    turntable_last_error_pos++;
    if (turntable_last_error_pos == NUM_PAST_ERRORS) 
    {
        turntable_last_error_pos = 0;
    }
}

/** Updates the register of past errors for the base arm angle
 *  @param error: the most recent error
 */
void base_arm_update_error(float error)
{
    base_arm_past_errors[base_arm_last_error_pos] = error;
    base_arm_last_error_pos++;
    if (base_arm_last_error_pos == NUM_PAST_ERRORS) 
    {
        base_arm_last_error_pos = 0;
    }
}

/** Updates the register of past errors for the forearm agnle
 *  @param error: the most recent error
 */
void forearm_update_error(float error)
{
    forearm_past_errors[forearm_last_error_pos] = error;
    forearm_last_error_pos++;
    if (forearm_last_error_pos == NUM_PAST_ERRORS)
    {
        forearm_last_error_pos = 0;
    }
}



