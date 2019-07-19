// TODO: add interrupts to periodically check on received commands. Respond COMM_BUSY if busy

#include <Arduino.h>

#include "CommandExecution.h"
#include "HardwareDefs.h"
#include "ArmController/ArmController.h"
#include "GauntletController/GauntletController.h"

//Commands to be received
#define GET_ACK 0x00
#define GET_ARM_POSITION 0x03

#define SET_TRAVEL_POSITION 0x04
#define SET_ASCENT_POSITION 0x05
#define SET_STONE_IN_GAUNTLET 0x06
#define CONFIRM_POST_PRESENCE 0x07
#define ASCEND_POST 0x08
#define GRAB_STONE 0x09
#define DEPLOY_GAUNTLET 0xA

// command status from Infinity
#define COMM_BUSY 0xAE
#define COMM_CORRUPT_COMMAND 0xAF

// start, stop, return code
#define START 0xAA
#define STOP 0x55

HardwareSerial CommSerial = HardwareSerial(RX, TX);

void send_response(byte *response, int response_length);

/** Initialize communications
 */
void init_communication(void) 
{
    CommSerial.begin(115200);
    while (!CommSerial) {
         Serial.println("you better not optimise me away, I'm USEFUL");
    }
    while (CommSerial.available()) {
        CommSerial.readString();
    }
    
    Serial.println("init_communication");
}

/** Executes commands sent by the blue pill, if they exist
 */
void execute_command(void) 
{
    if (CommSerial.available() < 3) {

        maintain_current_arm_position();

        return;
    }
    byte param;

    byte start = CommSerial.read();
    Serial.print("start: ");
    Serial.println(start);
    byte command = CommSerial.read();
    Serial.print("command: ");
    Serial.println(command);
    if (command == CONFIRM_POST_PRESENCE) {
        param = CommSerial.read();
    }
    byte stop = CommSerial.read();
    Serial.print("stop: ");
    Serial.println(stop);

    if (start != START || STOP != STOP) {
        byte corrupted[1] = {COMM_CORRUPT_COMMAND};
        send_response(corrupted, 1);
    }
    byte response[1];

    switch(command) {
        case GET_ACK:
        {
            Serial.println("GET_ACK");
            byte success[1] = {COMM_SUCCESS};
            send_response(success, 1); // temp placeholder responses
            break;
        }
        case GET_ARM_POSITION:
        {
            Serial.println("GET_ARM_POSITION");
            byte arm_position_info[5];
            response[0] = COMM_SUCCESS;
            if (get_arm_position(&response[1]) != SUCCESS) {
                response[0] = COMM_TASK_FAILED;
                send_response(arm_position_info, 1);
            }
            send_response(response, 5);
            break;
        }
        case SET_TRAVEL_POSITION:
        {
            Serial.println("SET_TRAVEL_POSITION");
            response[0] = position_arm_for_travel();
            send_response(response, 1);
            break;
        }
        case SET_ASCENT_POSITION:
        {
            Serial.println("SET_ASCENT_POSITION");
            response[0] = position_arm_for_ascent();
            send_response(response, 1);
            break;
        }
        case SET_STONE_IN_GAUNTLET:
        {
            Serial.println("SET_STONE_IN_GAUNTLET");
            gauntlet_open_position();
            response[0] = put_stone_in_gauntlet();
            gauntlet_storage_position();
            send_response(response, 1);
            break;
        }
        case CONFIRM_POST_PRESENCE:
        {

            Serial.println("CONFIRM_POST_PRESENCE");
            response[0] = find_post(param);
            send_response(response, 1);
            break;
        }
        case ASCEND_POST:
        {
            Serial.println("ASCEND_POST");
            response[0] = ascend_post_to_top();
            send_response(response, 1);
            break;
        }
        case GRAB_STONE:
        {
            Serial.println("GRAB_STONE");
            response[0] = grab_infinity_stone();
            send_response(response, 1);
            break;
        }
        case DEPLOY_GAUNTLET:
        {
            Serial.println("DEPLOY_GAUNTLET");
            response[0] = gauntlet_deploy_position();
            send_response(response, 1);
            break;
        }
        default:
        {
            Serial.println("command not recognised");
            byte corrupted[1] = {COMM_CORRUPT_COMMAND};
            send_response(corrupted, 1);
            break; 
        }
    }
}

/** Sends some response
 * Add some error catching / retries after
 */
void send_response(byte *response, int response_length) 
{
    byte response_buff[2 + response_length];
    response_buff[0] = START;
    int i;
    for (i = 0; i < response_length; i++) {
        response_buff[i + 1] = response[i];
    }
    response_buff[response_length + 1] = STOP;

    while (!CommSerial.availableForWrite());
    CommSerial.write(response_buff, response_length + 2);
}