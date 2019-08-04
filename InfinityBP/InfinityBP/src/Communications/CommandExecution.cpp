// TODO: add interrupts to periodically check on received commands. Respond COMM_BUSY if busy

#include <Arduino.h>

#include "Communications/CommandExecution.h"
#include "GlobalInfo/HardwareDefs.h"
#include "ArmController/ArmController.h"
#include "GauntletController/GauntletController.h"

//Commands to be received
#define GET_ACK 0x00
#define GET_ARM_POSITION 0x03

#define SET_STONE_IN_GAUNTLET 0x06
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
    
    Serial.println("CommandExecution initialized");
}

/** Executes commands sent by the blue pill, if they exist
 */
void execute_command(void) 
{

    //If there are less than 3 bytes in the buffer, no message is through yet
    if (CommSerial.available() < 3) {

        maintain_current_arm_position();

        return;
    }
    byte side, post_number;

    byte start = CommSerial.read();

    byte command = CommSerial.read();

    if (command == GRAB_STONE)
    {
        while (CommSerial.available() < 3);

        side = CommSerial.read();
        post_number = CommSerial.read();

    }

    byte stop = CommSerial.read();

    #if DEBUG_ALL

        Serial.println();

        Serial.print("received message: ");

        Serial.print("  ");
        Serial.print(start);

        Serial.print("  ");
        Serial.print(command);

        if (command == GRAB_STONE)
        {
            Serial.print("  ");
            Serial.print(side);

            Serial.print("  ");
            Serial.print(post_number);
        }

        Serial.print("  ");
        Serial.print(stop);

        Serial.println();

    #endif

    if (start != START || stop != STOP) {
        byte corrupted[1] = {COMM_CORRUPT_COMMAND};
        send_response(corrupted, 1);
    }
    byte response[1];

    switch(command) {
        
        //Establish communication
        case GET_ACK:
        {

            #if DEBUG_ALL
                Serial.println("GET_ACK");
            #endif

            byte success[1] = {COMM_SUCCESS};

            send_response(success, 1);
            break;
        }

        case GET_ARM_POSITION:
        {

            #if DEBUG_ALL
                Serial.println("GET_ARM_POSITION");
            #endif

            byte arm_position_info[5];

            arm_position_info[0] = COMM_SUCCESS;

            if (get_arm_position(&arm_position_info[1]) != SUCCESS) {
                arm_position_info[0] = COMM_TASK_FAILED;
                send_response(arm_position_info, 1);
            }

            send_response(arm_position_info, 5);
            break;
        }

        case SET_STONE_IN_GAUNTLET:
        {

            #if DEBUG_ALL
                Serial.println("SET_STONE_IN_GAUNTLET");
            #endif
            
            response[0] = gauntlet_open_position();

            response[0] = put_stone_in_gauntlet();
            
            response[0] = gauntlet_storage_position();

            send_response(response, 1);
            break;
        }

        case GRAB_STONE:
        {
            #if DEBUG_ALL
                Serial.println("GRAB_STONE");
            #endif

            response[0] = obtain_infinity_stone(side, post_number);

            send_response(response, 1);

            break;
        }

        case DEPLOY_GAUNTLET:
        {
            #if DEBUG_ALL
                Serial.println("DEPLOY_GAUNTLET");
            #endif

            response[0] = gauntlet_deploy_position();

            send_response(response, 1);

            break;
        }

        default:
        {
            #if DEBUG_ALL
                Serial.println("command not recognised");
            #endif

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

    #if DEBUG_ALL

        Serial.println();
        
        Serial.print("response sent: ");

        for (i = 0; i < response_length + 2; i++)
        {
            Serial.print(response_buff[i]);
            Serial.print("  ");
        }
        Serial.println();

    #endif


}