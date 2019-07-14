#include <Arduino.h>

#include "CommandExecution.h"
#include "Arm.h"
#include "Gauntlet.h"
#include "HardwareDefs.h"

//Commands to be received
#define GET_ACK 0x00
#define GET_ARM_POSITION 0x03

#define SET_TRAVEL_POSITION 0x04
#define SET_ASCENT_POSITION 0x05
#define SET_STONE_IN_GAUNTLET 0x06
#define CONFIRM_POST_PRESENCE 0x07
#define ASCEND_POST 0x08
#define GRAB_STONE 0x09

// communication responses
#define ACK 0xAA // cool all went well OR ok i'll try
#define FAIL 0xAB // task failed (e.g. post not found)
#define BUSY 0xAC // haven't completed the previous task CHILL
#define CORRUPTED 0xAD // value received made no sense

HardwareSerial CommSerial = HardwareSerial(RX, TX);

void init_communication() 
{
    CommSerial.begin(115200);
    while (!CommSerial);
    
    Serial.println("init_communication");
}

void execute_command() 
{
    if (!CommSerial.available()) {
        return;
    }
    byte command = CommSerial.read();

    switch(command) {
        case GET_ACK:
            break;
        case SET_TRAVEL_POSITION:
            break;
        case SET_ASCENT_POSITION:
            break;
    }


}