#include "GlobalInfo/HardwareDefs.h"
#include "Locomotion/PID.h"

// PID related values
#define NUM_PAST_ERRORS 10 // number of errors to save
#define MAX_ANALOG 1023
#define PIN_PROP PB0
#define PIN_DERIV PB1

// declare values necessary for calculations
int past_errors[NUM_PAST_ERRORS];
int last_error_index;
int kp, kd;

float getP(int error);
float getD(int error);
void updateError(int error);

/**
 * Initialises PID module
 * Creates empty array of past errors
 * Sets initial values for kp, kd (hardcoded)
 */
void init_PID()
{
    Serial.println("init_PID");
    int i = 0;
    for (i = 0; i < NUM_PAST_ERRORS; i++) {
        past_errors[i] = 0;
    }
    last_error_index = 0;
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
    Serial.println("get_PID_output");
    float out = getP(error) + getD(error);

    updateError(error);

    if (out > 0.5) {
        out = 0.5;
    }
    return out;
}

/**
 * Reads kp value from debugging potentiometer, sets it as new kp
 * Returns: kp value set
 */
int update_kp() 
{
    kp = analogRead(CALIBRATION_POTENTIOMETER);
    return kp;
}

/**
 * Reads kd value from debugging potentiometer, sets it as new kd
 * Returns: kd value set
 */
int update_kd()
{
    kd = analogRead(CALIBRATION_POTENTIOMETER);
    return kd;
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

/**
 * Returns: correction due to proportional error
 */
float getP(int error)
{
    return error * kp / MAX_ANALOG;
}

/**
 * Returns: correction due to derivative error
 */
float getD(int error)
{
    return (error - past_errors[last_error_index]) * kd / MAX_ANALOG;
}

/**
 * Updates record of 10 previous errors
 * This is so that we can get better derivative estimates for D
 * Deals with threshold
 */
void updateError(int error)
{
    past_errors[last_error_index] = error;
    last_error_index++;
    if (last_error_index == NUM_PAST_ERRORS) {
        last_error_index = 0;
    }
}