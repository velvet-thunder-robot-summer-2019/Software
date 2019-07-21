#ifndef HARDWAREDEFS_H_
#define HARDWAREDEFS_H_

#include <Arduino.h>

/* Pin mapping */
//DEBUG BOARD
#define MASTER_SWITCH PB15 // green switch
#define THANOS_v_METHANOS_SWITCH PB14 // red switch
#define NAVIGATE PB12
#define SET PB13
#define CALIBRATION_POTENTIOMETER PA7

//bumpers
#define BUMPER_FRONT PB3
#define BUMPER_BACK PA15
#define BUMPER_LEFT PA12
#define BUMPER_RIGHT PA11

//tape following
#define TORQUE_OF_MOTION_AVG 10

//sensors
#define OUTER_LEFT_SENSOR PA2
#define LEFT_SENSOR PA3
#define RIGHT_SENSOR PA4
#define OUTER_RIGHT_SENSOR PA5

#define LEFT_WING_SENSOR PA1
#define RIGHT_WING_SENSOR PA0

//motors
#define RIGHT_MOTOR_PWM_BACK PB_9
#define RIGHT_MOTOR_PWM_FWD PB_8

#define LEFT_MOTOR_PWM_BACK PB_1
#define LEFT_MOTOR_PWM_FWD PB_0

//encoders
#define ENCODER_RIGHT_A PB9
#define ENCODER_RIGHT_B PA8
#define ENCODER_LEFT_A PB5
#define ENCODER_LEFT_B PB4

// fault light
#define BLINKY PC13

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
#define BOTH_SIDES 2
#define FWD 1
#define BACK 0

#define ON_TAPE 0
#define LEFT_OFF_RIGHT_ON -1
#define RIGHT_OFF_LEFT_ON 1
#define BOTH_OFF_LAST_RIGHT -5
#define BOTH_OFF_LAST_LEFT 5

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

// drivings vals and PWMs
#define FLAT_GROUND_TAPE_FOLLOWING_PWM 0.3
#define FLAT_GROUND_APPROACHING_STOP_PWM 0.05
#define ARC_LENGTH_FOR_TURN 100
#define TURN_PWM 0.1

// communication status
// when communication fails
#define COMM_TIMEOUT 0xAA // i.e. no response
#define COMM_CORRUPT_RESPONSE 0xAB

// command status from Infinity
#define COMM_SUCCESS 0xAC // note that success can ALSO double as meaning acknowledged, for those tasks that don't wait on completion to reply
#define COMM_TASK_FAILED 0xAD
#define COMM_BUSY 0xAE
#define COMM_CORRUPT_COMMAND 0xAF


// just for testing w/o bot
#define TESTING_ORDER_OF_EVENTS TRUE

#endif // HARDWAREDEFS_H_