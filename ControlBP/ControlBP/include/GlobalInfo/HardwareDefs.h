#include <Arduino.h>

/* Pin mapping */
//DEBUG BOARD
#define MASTER_SWITCH PB13
#define THANOS_v_METHANOS_SWITCH PB12
#define NAVIGATE PB15
#define SET PB14
#define CALIBRATION_POTENTIOMETER PB1

//bumpers
#define BUMPER_FRONT PB3
#define BUMPER_BACK PA15
#define BUMPER_LEFT PA12
#define BUMPER_RIGHT PA11



/* Here lie definitions to make code more legible */
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