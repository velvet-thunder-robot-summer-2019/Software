#include "CommandExecution.h"
#include "Arm.h"
#include "Gauntlet.h"

//Commands to be received
#define GET_ACK 0x00
#define GET_U_TURN_STATUS 0x01
#define GET_LEFT_WHEEL_ROT 0x02
#define GET_RIGHT_WHEEL_ROT 0x03

#define SET_TRAVEL_POSITION 0x04
#define SET_ASCENT_POSITION 0x05
#define SET_STONE_IN_GAUNTLET 0x06
#define CONFIRM_POST_PRESENCE 0x07
#define ASCEND_POST 0x08
#define GRAB_STONE 0x09
