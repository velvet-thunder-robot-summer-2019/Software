/** This module translates expected arm angles into movements, and applies PID control to 
 *      correct position
 */

#define MOVE_SUCCESS 1
#define MOVE_FAIL 0

#define FORWARD 1
#define BACKWARD 0

#define CLAW_SERVO_PIN 0
#define WRIST_SERVO_PIN 0

#define CLAW_SERVO_OPEN 0
#define CLAW_SERVO_CLOSE 0

#define WRIST_SERVO_MID 0

#define TURNTABLE_POS_PIN 0
#define TURNTABLE_NEG_PIN 0

#define BASE_ARM_POS_PIN 0
#define BASE_ARM_NEG_PIN 0

#define FORE_ARM_POS_PIN 0
#define FORE_ARM_NEG_PIN 0

#define PWM_CLOCK_FREQ 0
#define PWM_PERIOD 0

enum motor_direction 
{
    CLOCKWISE,
    ANTI_CLOCKWISE
};

struct pwm_response 
{
    motor_direction dir;
    float   pwm_val;
}

/** Move the arm to the position specified by the angles
 *  @param base_arm_angle: angle of the base arm clockwise from the z-axis in degrees
 *  @param forearm_angle: angle of the forearm clockwise from the direction of the base arm in degrees.
 *  @param wrist_angle: angle of the wrist clockwise from the direction of the forearm in degrees
 *  @param turntable_angle: The turning angle of the turntable from the x axis in degrees.
 *  Returns: MOVE_SUCCESS if the required configuration is reached
 *           MOVE_FAILED if the required configuration cannot be obtained
 */
byte move_whole_arm_position(float base_arm_angle, float forearm_angle, float wrist_angle, float turntable_angle);

/** Opens the claw
 */
void open_claw(void);

/** Closes the claw
 */
void close_claw(void);

/** Calculate a PID pwm response to the angular difference of the turntable
 *  @param delta_turntable_angle: difference between expected turntable angle and actual potentiometer value
 *  Returns: pwm_response struct
 */
pwm_response calculate_turntable_pwm(float delta_turntable_angle);

/** Calculate a PID pwm response to the angular difference of the base arm
 *  @param delta_base_arm_angle: difference between expected base arm angle and actual potentiometer value
 *  Returns: pwm_response struct
 */
pwm_response calculate_base_arm_pwm(float delta_base_arm_angle);

/** Calculate a PID pwm response to the angular difference of the forearm
 *  @param delta_forearm_angle: difference between expected forearm angle and actual potentiometer value
 *  Returns: pwm_response_struct
 */
pwm_response calculate_forearm_pwm(float delta_forearm_angle);

/** Updates the register of past errors for the turntable angle
 *  @param error: the most recent error
 */
void turntable_update_error(float error);

/** Updates the register of past errors for the base arm angle
 *  @param error: the most recent error
 */
void base_arm_update_error(float error);

/** Updates the register of past errors for the forearm agnle
 *  @param error: the most recent error
 */
void forearm_update_error(float error);
