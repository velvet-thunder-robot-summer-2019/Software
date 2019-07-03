#include "Initialisation/HardwareDefs.h"
#include "Locomotion/PID.h"

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