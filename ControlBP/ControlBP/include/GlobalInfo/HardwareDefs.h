#include <Arduino.h>

/* Pin mapping */
//DEBUG BOARD
#define MASTER_SWITCH PB13
#define THANOS_v_METHANOS_SWITCH PB12
#define NAVIGATE PB15
#define SET PB14
#define CALIBRATION_POTENTIOMETER PA7

//bumpers
#define BUMPER_FRONT PB3
#define BUMPER_BACK PA15
#define BUMPER_LEFT PA12
#define BUMPER_RIGHT PA11

//tape following
#define TORQUE_OF_MOTION_AVG 10

//sensors
#define LEFT_SENSOR PA4
#define RIGHT_SENSOR PA5

//motors
#define RIGHT_MOTOR_PWM_BACK PB_9
#define RIGHT_MOTOR_PWM_FWD PB_8

#define LEFT_MOTOR_PWM_BACK PB_1
#define LEFT_MOTOR_PWM_FWD PB_0

//communication
#define TX PB10
#define RX PB11

/* Here lie definitions to make code more legible */
// calibration output
#define CALIBRATION_DELTA_TO_PRINT 10

//syntactical sugar
#define SUCCESS 0
#define STATE_CHANGED 1
#define TRUE 1
#define FALSE 0
#define OPEN 0
#define CLOSED 1

// state change handling
#define STATE_CHANGE_SUCCESSFUL 0
#define OTHER_STATE_CHANGE_IN_PROGRESS 1
#define OTHER_STATE_CHANGE_OCCURRED_FIRST 2

// tape sensor and driving
#define TAPE_NOT_FOUND 2
#define LEFT 0
#define RIGHT 1
#define FWD 1
#define BACK 0

#define ON_TAPE 0
#define LEFT_OFF_RIGHT_ON -1
#define RIGHT_OFF_LEFT_ON 1
#define BOTH_OFF_LAST_RIGHT -5
#define BOTH_OFF_LAST_LEFT 5

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

// communication status
#define TIMEOUT -1
#define CORRUPT_RESPONSE -2

#define ACK 0xAA // cool all went well OR ok i'll try
#define FAIL 0xAB // task failed (e.g. post not found)
#define BUSY 0xAC // haven't completed the previous task CHILL
#define NO_RESPONSE 0xAD // Infinity is ghosting us