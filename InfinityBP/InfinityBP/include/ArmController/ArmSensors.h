#define TAPE_SENSOR_THRESHOLD 0
#define TAPE_SENSOR 0

#define TURNTABLE_POTENTIOMETER 0
#define BASE_ARM_POTENTIOMETER 0
#define FORE_ARM_POTENTIOMETER 0

#define TURNTABLE_SCALE_FACTOR 0
#define BASE_ARM_SCALE_FACTOR 0
#define FORE_ARM_SCALE_FACTOR 0

#define TURNTABLE_ANGLE_OFFSET 0
#define BASE_ARM_ANGLE_OFFSET 0
#define FORE_ARM_ANGLE_OFFSET 0

/** Initialize all the sensors on the arm
 */
void init_arm_sensors(void);

/** Get an analog value for the tape sensor reading
 *  Returns: A tape sensor value between 0 and 1023
 */
uint16_t read_tape_sensor_analog(void);

/** Get a digital reading for the tape sensor reading
 *  Returns: A boolean reading for tape sensor
 */
uint16_t read_tape_sensor_digital(void);

/** Read the turntable angle in degrees
 *  Returns: The turntable angle in degrees, between 0 and 365
 */
float read_turntable_angle(void);

/** Read the base arm angle in degrees
 *  Returns: The base arm angle in degrees, between -180 to +180
 */
float read_base_arm_angle(void);

/** Read the base arm angle in degrees
 *  Returns: The fore arm angle in degrees, between -180 and +180
 */
float read_fore_arm_angle(void);