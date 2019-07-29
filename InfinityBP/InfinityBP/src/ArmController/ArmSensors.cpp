#include "ArmController/ArmSensors.h"
#include "GlobalInfo/HardwareDefs.h"

/** Initialize all the sensors on the arm
 */
void init_arm_sensors(void)
{
    //Initialize the tape sensor
    pinMode(TAPE_SENSOR, INPUT);

    //Initialize the arm potentiometers
    pinMode(TURNTABLE_POTENTIOMETER, INPUT);
    pinMode(BASE_ARM_POTENTIOMETER, INPUT);
    pinMode(FORE_ARM_POTENTIOMETER, INPUT);
}

/** Get an analog value for the tape sensor reading
 *  Returns: A tape sensor value between 0 and 1023
 */
uint16_t read_tape_sensor_analog(void)
{
    return analogRead(TAPE_SENSOR);
}

/** Get a digital reading for the tape sensor reading
 *  Returns: A boolean reading for tape sensor
 */
uint16_t read_tape_sensor_digital(void)
{
    return analogRead(TAPE_SENSOR) > TAPE_SENSOR_THRESHOLD;
}

/** Read the turntable angle in degrees
 *  Returns: The turntable angle in degrees, between 0 and 365
 */
float read_turntable_angle(void)
{
    return analogRead(TURNTABLE_POTENTIOMETER) * TURNTABLE_POTENTIOMETER - TURNTABLE_ANGLE_OFFSET;
}

/** Read the base arm angle in degrees
 *  Returns: The base arm angle in degrees, between 180 CCW from z-axis to 180 CW from z-axis
 */
float read_base_arm_angle(void)
{
    return (float) ( (int32_t) analogRead(BASE_ARM_POTENTIOMETER) - 349) / 4.34;
    //return analogRead(BASE_ARM_POTENTIOMETER) * BASE_ARM_SCALE_FACTOR - BASE_ARM_ANGLE_OFFSET;
}

/** Read the base arm angle in degrees
 *  Returns: The fore arm angle in degrees, between 180 CCW from z-axis to 180 CW from z-axis
 */
float read_fore_arm_angle(void)
{

    //Serial.println(analogRead(FORE_ARM_POTENTIOMETER));
    return (float) (648 - (int32_t) analogRead(FORE_ARM_POTENTIOMETER)) / 4.75;
    //return analogRead(FORE_ARM_POTENTIOMETER) * FORE_ARM_SCALE_FACTOR - FORE_ARM_ANGLE_OFFSET;
}

