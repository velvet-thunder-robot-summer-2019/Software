#include "GlobalInfo/HardwareDefs.h"
#include "Locomotion/PID.h"

// PID related values
#define NUM_PAST_ERRORS 10 // number of errors to save
#define MAX_ANALOG 1023.0
#define PIN_PROP PB0
#define PIN_DERIV PB1

// declare values necessary for calculations
// int past_errors[NUM_PAST_ERRORS];
int past_error = 0;
// int oldest_error_index;
int kp, kd;

float getP(int error);
float getD(int error);
void updateError(int error);

// for driving around at the top: kp = 295, kd = 250



/**
 * Initialises PID module
 * Creates empty array of past errors
 * Sets initial values for kp, kd (hardcoded)
 */
void init_PID()
{
    // Serial.println("init_PID");
    // int i = 0;
    // for (i = 0; i < NUM_PAST_ERRORS; i++) {
    //     past_errors[i] = 0;
    // }
    // oldest_error_index = 0;
    set_kd_kp_for_ramp();
}

/**
 * Returns PID value which determines wanted torque, updates array of past errors
 * Params: error - the tape following error value
 * Returns: wanted torque offset (to add to 1 motor, subtract to other)
 */
float get_PID_output(int error)
{
    // Serial.println("get_PID_output");
    float out = getP(error) + getD(error);

    past_error = error;

    // updateError(error);

    return out;
}

/**
 * Reads kp value from debugging potentiometer, sets it as new kp
 * Returns: kp value set
 */
int update_kp() 
{
    // Serial.print("kp being updated ALERT");

    kp = analogRead(CALIBRATION_POTENTIOMETER);
    return kp;
}

/**
 * Reads kd value from debugging potentiometer, sets it as new kd
 * Returns: kd value set
 */
int update_kd()
{
    // Serial.print("kd being updated ALERT");
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
    return (error * kp * 0.2) / MAX_ANALOG;
}

/**
 * Returns: correction due to derivative error
 */
float getD(int error)
{
    /*
    Serial.print("D value is: ");
    Serial.println((float)(error - past_errors[last_error_index]) * kd * 0.05 / MAX_ANALOG);
    Serial.println("");
    */

    // return (float) (error - past_errors[oldest_error_index]) * kd * 0.5 / MAX_ANALOG;
    return (float) (error - past_error) * kd * 0.2  / MAX_ANALOG;
}

void set_kd_kp_for_post_nav() {
    kp = 295;
    kd = 250;
}

void set_kd_kp_for_reach_ramp() {
    // kp = 450, kd = 0
    kp = 400;
    kd = 100;
}

void set_kd_kp_for_ramp() {
    kp = 100;
    kd = 0;
}


// /**
//  * Updates record of 10 previous errors 
//  * This is so that we can get better derivative estimates for D
//  * Deals with threshold
//  */
// void updateError(int error)
// {
//     past_errors[oldest_error_index] = error;
//     oldest_error_index++;
//     if (oldest_error_index == NUM_PAST_ERRORS) {
//         oldest_error_index = 0;
//     }
// }