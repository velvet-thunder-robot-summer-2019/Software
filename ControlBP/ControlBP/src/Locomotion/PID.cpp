#include "GlobalInfo/HardwareDefs.h"
#include "Locomotion/PID.h"

int kp;
int kd;


/**
 * Initialises PID module
 * Creates empty array of past errors
 * Sets initial values for kp, kd (hardcoded)
 */
void init_PID()
{
    Serial.println("init_PID");
    kp = 0;
    kd = 0;
}

/**
 * Returns PID value which determines wanted torque, updates array of past errors
 * Params: error - the tape following error value
 * Returns: wanted torque offset (to add to 1 motor, subtract to other)
 */
float get_PID_output(int error)
{
    Serial.println("init_PID");
    return 0.0;
}

/**
 * Reads kp value from debugging potentiometer, sets it as new kp
 * Returns: kp value set
 */
int update_kp() 
{
    return 0;
}

/**
 * Reads kd value from debugging potentiometer, sets it as new kd
 * Returns: kd value set
 */
int update_kd()
{
    return 0;
}

/**
 * Returns current kp value
 */
int get_kp()
{
    return kp;
}


/**
 * Returns current kd value
 */
int get_kd()
{
    return kd;
}