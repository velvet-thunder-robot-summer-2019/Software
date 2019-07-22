#ifndef HARDWARE_DEFS_H_
#define HARDWARE_DEFS_H_

#include <Arduino.h>

/**
 * PID Tuning Variables
 */
#define TURNTABLE_KP 0
#define BASE_ARM_KP 0
#define FOREARM_KP 0

#define TURNTABLE_KI 0
#define BASE_ARM_KI 0
#define FOREARM_KI 0

#define TURNTABLE_KD 0
#define BASE_ARM_KD 0
#define FOREARM_KD 0

#define TURNTABLE_ANTI_WINDUP 0
#define BASE_ARM_ANTI_WINDUP 0
#define FOREARM_ANTI_WINDUP 0

#define NUM_PAST_ERRORS 10

#define ANGULAR_ERROR_BOUND 0

/**
 * Hardware Pinout
 */

//Communication Pins
#define TX PB10
#define RX PB11

//Gauntlet servo pin
#define GAUNTLET_SERVO_PIN PA_0

//Potentiometer pins
#define TURNTABLE_POTENTIOMETER PA_1
#define BASE_ARM_POTENTIOMETER PA_2
#define FORE_ARM_POTENTIOMETER PA_3

#define CLAW_SERVO_PIN PA_4

#define WRIST_SERVO_PIN PA_5

#define TURNTABLE_POS_PIN PB_1
#define TURNTABLE_NEG_PIN PB_2

#define BASE_ARM_POS_PIN PB_3
#define BASE_ARM_NEG_PIN PB_4

#define FORE_ARM_POS_PIN PB_5
#define FORE_ARM_NEG_PIN PB_6

#define TAPE_SENSOR PB_7

/**
 * Angular and lengthwise positioning
 */

//Gauntlet positions
#define GAUNTLET_STORAGE_POS 0
#define GAUNTLET_OPEN_POS 0
#define GAUNTLET_DEPLOY_POS 0

//Potentiometer angle mapping
#define TURNTABLE_SCALE_FACTOR 0
#define BASE_ARM_SCALE_FACTOR 0
#define FORE_ARM_SCALE_FACTOR 0

#define TURNTABLE_ANGLE_OFFSET 0
#define BASE_ARM_ANGLE_OFFSET 0
#define FORE_ARM_ANGLE_OFFSET 0

//The angles needed to bring the arm to its travel position in degrees
#define TURNTABLE_TRAVEL_ANGLE 0
#define BASE_ARM_TRAVEL_ANGLE 0
#define FORE_ARM_TRAVEL_ANGLE 0
#define WRIST_TRAVEL_ANGLE 0

//Arm lengths in mm
#define L1 20
#define L2 20
#define L3 5

//The angles needed to bring the arm to its ascent position in degrees
#define TURNTABLE_ASCENT_ANGLE 0
#define BASE_ARM_ASCENT_ANGLE 0
#define FORE_ARM_ASCENT_ANGLE 0
#define WRIST_ASCENT_ANGLE 0

//The angles needed to bring the arm to a search position in degrees
#define BASE_ARM_SEARCH_ANGLE 0
#define FORE_ARM_SEARCH_ANGLE 0
#define WRIST_SEARCH_ANGLE 0

//The starting point, and the search arc for the arm in degrees
#define TURNTABLE_SEARCH_LEFT 0 
#define TURNTABLE_SEARCH_RIGHT 0
#define TURNTABLE_SEARCH_ARC 0
#define TURNTABLE_STEP_RESOLUTION 0

//Stone post heights in mm
#define POST_1_HEIGHT 0
#define POST_2_HEIGHT 0
#define POST_3_HEIGHT 0
#define POST_4_HEIGHT 0
#define POST_5_HEIGHT 0
#define POST_6_HEIGHT 0

//Definitive list of XYZ positions in mm of the gauntlet at its open position
#define GAUNTLET_POS_1_X 0
#define GAUNTLET_POS_1_Y 0
#define GAUNTLET_POS_1_Z 0

#define GAUNTLET_POS_2_X 0
#define GAUNTLET_POS_2_Y 0
#define GAUNTLET_POS_2_Z 0

#define GAUNTLET_POS_3_X 0
#define GAUNTLET_POS_3_Y 0
#define GAUNTLET_POS_3_Z 0

#define GAUNTLET_POS_4_X 0
#define GAUNTLET_POS_4_Y 0
#define GAUNTLET_POS_4_Z 0

#define GAUNTLET_POS_5_X 0
#define GAUNTLET_POS_5_Y 0
#define GAUNTLET_POS_5_Z 0

#define GAUNTLET_POS_6_X 0
#define GAUNTLET_POS_6_Y 0
#define GAUNTLET_POS_6_Z 0

//vertical distance in mm needed to clear the arm
#define Z_PULLUP_DISTANCE 0

//increment value in mm to search for the top of the post
#define VERTICAL_STEP_RESOLUTION 0

//horizontal distance needed to reach the stone
#define EXTENSION_LENGTH 0

//claw angle positions
#define CLAW_SERVO_OPEN 0
#define CLAW_SERVO_CLOSE 0

//servo angle positions
#define WRIST_SERVO_MID 0

/**
 * Definitions
 */

//Angle conversions
#define RAD_DEG 180.0/3.1415926535
#define DEG_RAD 3.1415925635/180.0

//Chirality
#define LEFT_SIDE  0
#define RIGHT_SIDE 1

//Tape sensor HIGH/LOW threshold
#define TAPE_SENSOR_THRESHOLD 0

//error codes
#define MOVE_SUCCESS 1
#define MOVE_FAIL 0

#define FORWARD 1
#define BACKWARD 0

//return this error if arm cannot reach this position
#define UNREACHABLE_ERROR 0

/**
 * PWM INIT
 */

//pwm initialization
#define PWM_CLOCK_FREQ 0
#define PWM_PERIOD 0

#endif
