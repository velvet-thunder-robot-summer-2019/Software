#include <Arduino.h>

/* Pin mapping */
//DEBUG BOARD
#define MASTER_SWITCH PB13
#define THANOS_v_METHANOS_SWITCH PB12

/* Here lie definitions to make code more legible */
//syntactical sugar
#define SUCCESS 0
#define STATE_CHANGED 1
#define TRUE 1
#define FALSE 0

// tape sensor and driving
#define TAPE_NOT_FOUND 2
#define LEFT 0
#define RIGHT 1
#define FWD 1
#define BACK 0