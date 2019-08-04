#ifndef HARDWARE_DEFS_H_
#define HARDWARE_DEFS_H_

#include <Arduino.h>

/** 
 * ###########################################
 * ########### PID Tuning Variables ##########
 * ###########################################
 */

#define BASE_ARM_DUTY_CYCLE 0.2
#define FORE_ARM_DUTY_CYCLE 0.2
#define TURNTABLE_DUTY_CYCLE 0

#define TURNTABLE_KP 0
#define BASE_ARM_KP 5

#define TURNTABLE_KI 0
#define BASE_ARM_KI 0

#define TURNTABLE_KD 0
#define BASE_ARM_KD 0

#define TURNTABLE_ANTI_WINDUP 0
#define BASE_ARM_ANTI_WINDUP 0

#define NUM_PAST_ERRORS 10

#define ANGULAR_ERROR_BOUND 3

/** 
 *  ######################################
 *  ########## Hardware Pinout ###########
 *  ######################################
 */

//Communication Pins
#define TX PB10
#define RX PB11

//Gauntlet servo pin
#define GAUNTLET_SERVO_PIN PA_0 //FALSE

//Potentiometer pins
#define TURNTABLE_POTENTIOMETER PA_5 //FALSE
#define BASE_ARM_POTENTIOMETER PA_2 //FALSE

#define CLAW_SERVO_PIN PB_7
#define WRIST_SERVO_PIN PB_6
#define FOREARM_SERVO_PIN PA_8

#define TURNTABLE_POS_PIN PB_1 //FALSE
#define TURNTABLE_NEG_PIN PB_2 //FALSE

#define BASE_ARM_CW_PIN PB_8
#define BASE_ARM_CCW_PIN PB_9

#define TAPE_SENSOR PB_7 //FALSE

/**
 * ################################################################
 * ############## Angular and lengthwise positioning ##############
 * ################################################################
 */

//Gauntlet positions
#define GAUNTLET_STORAGE_POS 6
#define GAUNTLET_OPEN_POS 10
#define GAUNTLET_DEPLOY_POS 15

//Potentiometer angle mapping
#define TURNTABLE_SCALE_FACTOR 1
#define BASE_ARM_SCALE_FACTOR 1

#define TURNTABLE_ANGLE_OFFSET 1
#define BASE_ARM_ANGLE_OFFSET 1

//The angles needed to bring the arm to its travel position in degrees
#define TURNTABLE_TRAVEL_ANGLE 0
#define BASE_ARM_TRAVEL_ANGLE 0
#define FORE_ARM_TRAVEL_ANGLE 0
#define WRIST_TRAVEL_ANGLE 0

//Arm lengths in mm
#define L1 200
#define L2 120
#define L3 150

//The angles needed to bring the arm to a search position in degrees
#define BASE_ARM_SEARCH_ANGLE 0
#define FORE_ARM_SEARCH_ANGLE 0
#define WRIST_SEARCH_ANGLE 0

//The starting point, and the search arc for the arm in degrees
#define TURNTABLE_SEARCH_LEFT 0 
#define TURNTABLE_SEARCH_RIGHT 0
#define TURNTABLE_SEARCH_ARC 0
#define TURNTABLE_STEP_RESOLUTION 0

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

//Definitive list of XYZ positions in mm of the posts relative to the arm base, when approached from
//    the left
#define POST_LEFT_1_X 0
#define POST_LEFT_1_Y 0
#define POST_LEFT_1_Z 0

#define POST_LEFT_2_X 0
#define POST_LEFT_2_Y 0
#define POST_LEFT_2_Z 0

#define POST_LEFT_3_X 0
#define POST_LEFT_3_Y 0
#define POST_LEFT_3_Z 0

#define POST_LEFT_4_X 0
#define POST_LEFT_4_Y 0
#define POST_LEFT_4_Z 0

#define POST_LEFT_5_X 0
#define POST_LEFT_5_Y 0
#define POST_LEFT_5_Z 0

#define POST_LEFT_6_X 0
#define POST_LEFT_6_Y 0
#define POST_LEFT_6_Z 0

//Definitive list of XYZ positions in mm of the posts relative to the arm base, when approached from
//  the right
#define POST_RIGHT_1_X 0
#define POST_RIGHT_1_Y 0
#define POST_RIGHT_1_Z 0

#define POST_RIGHT_2_X 0
#define POST_RIGHT_2_Y 0
#define POST_RIGHT_2_Z 0

#define POST_RIGHT_3_X 0
#define POST_RIGHT_3_Y 0
#define POST_RIGHT_3_Z 0

#define POST_RIGHT_4_X 0
#define POST_RIGHT_4_Y 0
#define POST_RIGHT_4_Z 0

#define POST_RIGHT_5_X 0
#define POST_RIGHT_5_Y 0
#define POST_RIGHT_5_Z 0

#define POST_RIGHT_6_X 0
#define POST_RIGHT_6_Y 0
#define POST_RIGHT_6_Z 0

#define LEFT_2_X 0
#define LEFT_2_Y 0
#define LEFT_2_Z 0

#define RIGHT_3_X 0
#define RIGHT_3_Y 0
#define RIGHT_3_Z 0

//vertical distance in mm needed to clear the arm
#define Z_PULLUP_DISTANCE 0

//increment value in mm to search for the top of the post
#define VERTICAL_STEP_RESOLUTION 0

//horizontal distance needed to reach the stone
#define EXTENSION_LENGTH 0

//claw angle positions
#define CLAW_SERVO_OPEN 15
#define CLAW_SERVO_CLOSE 10

//servo angle positions
#define WRIST_SERVO_MID 0

/**
 * #############################################
 * ############## Definitions ##################
 * #############################################
 */

//Chirality
#define LEFT_SIDE  0x00
#define RIGHT_SIDE 0xAA

#define TRUE 1
#define FALSE 0

//Tape sensor HIGH/LOW threshold
#define TAPE_SENSOR_THRESHOLD 1

//error codes
#define MOVE_SUCCESS 1
#define MOVE_FAIL 0

#define FORWARD 1
#define BACKWARD 0

//return this error if arm cannot reach this position
#define UNREACHABLE_ERROR -999

/**
 * ##########################
 * ###### PWM INIT ##########
 * ##########################
 */

//pwm initialization
#define PWM_CLOCK_FREQ 100000
#define PWM_PERIOD 500

#define SERVO_FREQ 10000
#define SERVO_PERIOD 200

/**
 * ###########################################
 * ############# Meta Config #################
 * ###########################################
 */

#define DEBUG_ALL TRUE
#define MOCK_HARDWARE FALSE
#define HARDCODE_POST_POSITIONS TRUE
#define ADAPTIVE_RESPONSE TRUE

#endif
