#ifndef PID_H_
#define PID_H_

/**
 * Initialises PID module
 * Creates empty array of past errors
 * Sets initial values for kp, kd (hardcoded)
 */
void init_PID();

/**
 * Returns PID value which determines wanted torque, updates array of past errors
 * Params: error - the tape following error value
 * Returns: wanted torque offset (to add to 1 motor, subtract to other)
 */
float get_PID_output(int error);

/**
 * Reads kp value from debugging potentiometer, sets it as new kp
 * Returns: kp value set
 */
int update_kp();

/**
 * Reads kd value from debugging potentiometer, sets it as new kd
 * Returns: kd value set
 */
int update_kd();

/**
 * Returns current kp value
 */
int get_kp();


/**
 * Returns current kd value
 */
int get_kd();

/**
 * Set values you want for navigation at the top
 */
void set_kd_kp_for_post_nav();

void set_kd_kp_for_ramp();

void set_kd_kp_for_reach_ramp();


#endif //PID_H_