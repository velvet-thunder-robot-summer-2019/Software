// TODO: implement a backtrack if we're "lost" requires a measure of lost.
// Get that

#include <Arduino.h>

#include "GlobalInfo/HardwareDefs.h"
#include "Locomotion/TapeSensor.h"

#define DEBUG_BRANCH_REACH_EXPECTED 20

int lastSensor = LEFT_SENSOR; // arbitrarily set, we just need one
int left_sensor();
int right_sensor();
int right_wing_sensor();
int left_wing_sensor();

uint32_t tape_sensor_threshold = 440;

/**
 * Initialising pins for tape sensing
 */
void initTapeSensor()
{
    pinMode(LEFT_SENSOR, INPUT);
    pinMode(RIGHT_SENSOR, INPUT);
}

/**
 * Returns the error in tape following from 2 front tape sensor input
 * (ON_TAPE, LEFT_OFF_RIGHT_ON, 
 * RIGHT_OFF_LEFT_ON, BOTH_OFF_LAST_RIGHT,
 * BOTH_OFF_LAST_LEFT)
 */
int get_tape_following_error()
{
    int left = left_sensor();
    int far_left = outer_left_sensor();
    int right = right_sensor();
    int far_right = outer_right_sensor();
#if DEBUG_PRINT
    Serial.println("get_tape_following_error");
    Serial.print("left sensor says: ");
    Serial.println(left);
    Serial.print("right sensor says: ");
    Serial.println(right);
#endif
    // delay(1000);

    if (left && right) {
        return ON_TAPE;
    } else if (right) {
        lastSensor = RIGHT_SENSOR;
        return LEFT_OFF_RIGHT_ON;
    } else if (left) {
        lastSensor = LEFT_SENSOR;
        return RIGHT_OFF_LEFT_ON;
    } else if (far_left) {
        lastSensor = OUTER_LEFT_SENSOR;
        return OUTER_LEFT;
    } else if (far_right) {
        lastSensor = OUTER_RIGHT_SENSOR;
        return OUTER_RIGHT;
    } else if (lastSensor == RIGHT_SENSOR) {
        return BOTH_OFF_LAST_RIGHT;
    } else if (lastSensor == LEFT_SENSOR) {
        return BOTH_OFF_LAST_LEFT;
    } else if (lastSensor == OUTER_LEFT_SENSOR) {
        return LAST_OUTER_LEFT;
    } else if (lastSensor == OUTER_RIGHT_SENSOR) {
        return LAST_OUTER_RIGHT;
    }
    // should never happen
    return LAST_OUTER_LEFT;
}

// int branch_reached()
// {
//     return left_wing_sensor() || right_wing_sensor();
// }

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
    bool branch_reached =  ((left_sensor() || right_sensor() || outer_right_sensor()) && outer_left_sensor()) ||
            ((left_sensor() || right_sensor() || outer_left_sensor()) && outer_right_sensor());
    // Serial.println(left_sensor());
    // Serial.println(right_sensor());
    // Serial.println(outer_left_sensor());
    // Serial.println(outer_right_sensor());

    // Serial.println((left_sensor() || right_sensor() || right_wing_sensor());
    // Serial.print(branch_reached);
    return branch_reached;
}

/**
 * Returns true if branch has been reached, false otherwise (as detected by wing sensors)
 * Params:  expected_side - side on which we expect a branch to appear
 */
int branch_reached(int expected_side)
{
#if TESTING_ORDER_OF_EVENTS
    Serial.print("branch_reached, expected side: ");
    Serial.println(expected_side);
#endif

    if ((expected_side == LEFT && left_wing_sensor()) ||
        (expected_side == RIGHT && right_wing_sensor()) ||
        (expected_side == BOTH_SIDES && (left_wing_sensor() || right_wing_sensor()))) {
        return TRUE;
    }
    return FALSE;
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
int left_sensor()
{
    // Serial.print("left sensor");
    // Serial.println(analogRead(LEFT_SENSOR));
    return analogRead(LEFT_SENSOR) > tape_sensor_threshold;
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
int right_sensor()
{
    // Serial.print("right sensor");
    // Serial.println(analogRead(RIGHT_SENSOR));
    return analogRead(RIGHT_SENSOR) > tape_sensor_threshold;
}

// /**
//  * Returns:     0 if sensor off
//  *              1 if sensor on
//  */
// int right_wing_sensor()
// {
//     return analogRead(RIGHT_WING_SENSOR) > tape_sensor_threshold;
// }

// /**
//  * Returns:     0 if sensor off
//  *              1 if sensor on
//  */
// int left_wing_sensor()
// {
//     return analogRead(LEFT_WING_SENSOR) > tape_sensor_threshold;
// }