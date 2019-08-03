// TODO: implement a backtrack if we're "lost" requires a measure of lost.
// Get that

#include <Arduino.h>

#include "GlobalInfo/HardwareDefs.h"
#include "Locomotion/TapeSensor.h"

int last_sensor = RIGHT; // arbitrarily set, we just need one


int last_stop_vals[4] = {0};

uint32_t tape_sensor_threshold = 660;

/**
 * Initialising pins for tape sensing
 */
void initTapeSensor()
{
    pinMode(INNER_LEFT_SENSOR, INPUT);
    pinMode(INNER_RIGHT_SENSOR, INPUT);
    pinMode(MID_RIGHT_SENSOR, INPUT);
    pinMode(MID_LEFT_SENSOR, INPUT);
    pinMode(OUTER_LEFT_SENSOR, INPUT);
    pinMode(OUTER_RIGHT_SENSOR, INPUT);
}

/**
 * Returns the error in tape following from 2 front tape sensor input
 * (ON_TAPE, LEFT_OFF_RIGHT_ON, 
 * RIGHT_OFF_LEFT_ON, BOTH_OFF_LAST_RIGHT,
 * BOTH_OFF_LAST_LEFT)
 */
int get_tape_following_error()
{

#if DEBUG_PRINT
    Serial.println("get_tape_following_error");
    Serial.print("left sensor says: "); 
    Serial.println(left);
    Serial.print("right sensor says: ");
    Serial.println(right);
#endif

    // option 1: safe option, should work
    int inner_left = inner_left_sensor();
    int inner_right = inner_right_sensor();
    int mid_left = mid_left_sensor();
    int mid_right = mid_right_sensor();

    if (inner_left && inner_right) {
        return 0;
    } else if (inner_left && mid_left) {
        last_sensor = LEFT;
        return -1;
    } else if (mid_left) {
        last_sensor = LEFT;
        return -2;
    } else if (inner_right && mid_right) {
        last_sensor = RIGHT;
        return 1;
    } else if (mid_right) {
        last_sensor = RIGHT;
        return -2;
    } else if (last_sensor == LEFT) {
        return 3;
    } else {
        return -3;
    }

    // option 2: I like it better, more resolution
    
}

/**
 * Returns TRUE if the front sensors reach a branch, FALSE otherwise
 * Essentially, if a side sensor and ANY other sensor are on tape, we can deduce that 
 * there must be a branch
 */
int branch_reached_front()
{
#if TESTING_ORDER_OF_EVENTS
Serial.println("branch reached front of robot");
return TRUE;
#endif
    int left = inner_left_sensor();
    int right = inner_right_sensor();
    int outer_left = outer_left_sensor();
    int outer_right = outer_right_sensor();


    bool branch_reached =  ((left || right || outer_right) && outer_left) ||
            ((left || right || outer_left) && outer_right) ||
            ((last_sensor == LEFT) && outer_right) ||
            ((last_sensor == RIGHT) && outer_left);
    if (branch_reached) {
        last_stop_vals[0] = outer_left;
        last_stop_vals[1] = left;
        last_stop_vals[2] = right;
        last_stop_vals[3] = outer_right;
    }
    // Serial.println(left_sensor());
    // Serial.println(right_sensor());
    // Serial.println(outer_left_sensor());
    // Serial.println(outer_right_sensor());

    // Serial.println((left_sensor() || right_sensor() || right_wing_sensor());
    // Serial.print(branch_reached);
    return branch_reached;
}

int last_stop_outer_left()
{
    return last_stop_vals[0];
}
int last_stop_outer_right() {
    return last_stop_vals[3];
}
int last_stop_left() {
    return last_stop_vals[1];
}
int last_stop_right() {
    return last_stop_vals[2];
}

/**
 * Updates cutoff threshold for tape sensors to potentiometer value
 */
void update_threshold_tape_sensor()
{
    Serial.println("update_threshold_tape_sensor");
    tape_sensor_threshold = analogRead(CALIBRATION_POTENTIOMETER);
}

/**
 * Returns the current threshold for tape sensor values
 */
int get_tape_sensor_threshold()
{
    return (int) tape_sensor_threshold;
}

/**
 * Returns: 0 if left sensor is over threshold (over white)
 *          1 if left sensor is below threshold (over tape)
 */
int inner_left_sensor()
{
    // Serial.print("left sensor");
    // Serial.println(analogRead(LEFT_SENSOR));
    return analogRead(INNER_LEFT_SENSOR) > tape_sensor_threshold;
}

/**
 * Returns: 0 if outer left sensor is over threshold (over white)
 *          1 if outer left sensor is below threshold (over tape)
 */
int outer_left_sensor()
{
    return analogRead(OUTER_LEFT_SENSOR) > tape_sensor_threshold;
}

/**
 * Returns: 0 if outer left sensor is over threshold (over white)
 *          1 if outer left sensor is below threshold (over tape)
 */
int outer_right_sensor()
{
    return analogRead(OUTER_RIGHT_SENSOR) > tape_sensor_threshold;
}


/**
 * Returns: 0 if right sensor is over threshold (over white)
 *          1 if right sensor is below threshold (over tape)
 */
int inner_right_sensor()
{
    return analogRead(INNER_RIGHT_SENSOR) > tape_sensor_threshold;
}

/**
 * mid sensors, same deal
 */
int mid_right_sensor()
{
    return analogRead(MID_RIGHT_SENSOR) > tape_sensor_threshold;
}
int mid_left_sensor()
{
    return analogRead(MID_LEFT_SENSOR) > tape_sensor_threshold;
}