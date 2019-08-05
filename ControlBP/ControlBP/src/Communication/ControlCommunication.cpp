// TODO: add state checks while we wait in these methods that take a while!!

#include <Arduino.h>

#include "GlobalInfo/GlobalVars.h"
#include "GlobalInfo/HardwareDefs.h"
#include "Communication/ControlCommunication.h"
#include "GlobalInfo/RobotState.h"

// Communication details
#define BUFFER_SIZE 7 // 2 for start/stop, 1 for return code, up to 4 for data
#define MAX_TIMEOUT 100001 //ms, max time to wait for availability
#define MAX_RETRIES_COMM 3 //max retries when a communication fails
#define WAIT_BETWEEN_COMM_RETRIES 5 

//Commands to Infinity 
//Arm commands
#define GET_ACK 0x00
#define GET_ARM_POSITION 0x03

#define SET_STONE_IN_GAUNTLET 0x06
#define GRAB_STONE 0x09
#define DEPLOY_GAUNTLET 0x0A

// start, stop, return code
#define START 0xAA
#define STOP 0x55

//TODO: Gauntlet commands

uint8_t send_command(byte command);
uint8_t get_response(byte byte_num);
uint8_t send_command_no_retries(byte command);
uint8_t get_response_no_retries(byte byte_num);
uint8_t send_command_get_stone_no_retries(byte side, byte post_num);
uint8_t send_command_get_stone(byte side, byte post_num);

HardwareSerial CommSerial = HardwareSerial(RX, TX);
byte infinity_response[BUFFER_SIZE];

/**
 * Initialises communication
 */
void init_communication()
{
    CommSerial.begin(115200);
    while (!CommSerial) {
#if DEBUG_PRINT
        Serial.println("don't let the compiler optimise me away!!");
#endif
    }
    while (CommSerial.available()) {
        CommSerial.readString();
    }
}

/**
 * Tests communication, makes a simple check to see if we can speak to other BP and get response
 * Returns:     COMM_SUCCESS if communication established and we received ACK
 *              Various error messages otherwise
 */
uint8_t establish_communication()
{
#if DEBUG_PRINT
    Serial.println("establish communication");
#endif
    uint8_t send_status = send_command(GET_ACK);
    if (send_status != COMM_SUCCESS) {
#if DEBUG_PRINT
        Serial.println("comm did not succeed");
        Serial.println(send_status);
#endif
        return send_status;
    }
    Serial.println("line sent");

    uint8_t response_status = get_response(1);
#if DEBUG_PRINT
    Serial.println("response received is:");
    Serial.println(infinity_response[0]);
    Serial.println(infinity_response[1]);
    Serial.println(infinity_response[2]);
#endif
    if (response_status == COMM_TIMEOUT) {
#if DEBUG_PRINT
    Serial.println("establish communication has timeout");
#endif
        return COMM_TIMEOUT;
    } else if (response_status == COMM_CORRUPT_RESPONSE) {
#if DEBUG_PRINT
        Serial.println("corrupt response");
#endif
        return COMM_CORRUPT_RESPONSE;
    }
    if (infinity_response[1] == COMM_SUCCESS) {
#if DEBUG_PRINT
        Serial.println("Initialised communication");
#endif
        return TRUE;
    }
    return COMM_CORRUPT_RESPONSE;
}

/**
 * Gives the angle of all joints in the following order: 
 * 0. lazy susan (360, but must manually scale)
 * 1. Shoulder (0 to 180)
 * 2. Elbow (0 to 180)
 * 3. Wrist (0 to 180)
 * Params:  angles - pointer to angle array
 */
uint8_t get_arm_angles(uint8_t *angles)
{
    uint8_t send_status = send_command(GET_ARM_POSITION);

    if (send_status == COMM_TIMEOUT) {
#if DEBUG_PRINT
        Serial.println("send timeout");
#endif
        return COMM_TIMEOUT;
    }
    uint8_t response_status = get_response(4);
    if (response_status == COMM_TIMEOUT) {
#if DEBUG_PRINT
        Serial.println("timeout");
#endif
        return COMM_TIMEOUT;
    } else if (response_status == COMM_CORRUPT_RESPONSE) {
#if DEBUG_PRINT
        Serial.println("corrupt");
#endif
        return COMM_CORRUPT_RESPONSE;
    }

    if (infinity_response[1] != COMM_SUCCESS) {
#if DEBUG_PRINT
        Serial.print("arm angle obtained");
#endif
        return infinity_response[1];
    }

    angles[0] = (uint8_t) infinity_response[2];
    angles[1] = (uint8_t) infinity_response[3];
    angles[2] = (uint8_t) infinity_response[4];
    angles[3] = (uint8_t) infinity_response[5];
#if DEBUG_PRINT
    Serial.println("angle received");
#endif
    return COMM_SUCCESS;
}

/**
 * Request put stone in gauntlet
 * Does not wait for reply
 */
uint8_t request_put_stone_in_gauntlet() 
{
#if DEBUG_PRINT || TESTING_ORDER_OF_EVENTS
    Serial.println("request_put_infinity_in_gauntlet: putting stone in gauntlet");
#endif
    uint8_t send_status = send_command(SET_STONE_IN_GAUNTLET);
    if (send_status == COMM_TIMEOUT) {
#if DEBUG_PRINT
        Serial.println("send timeout");
#endif
        return COMM_TIMEOUT;
    }

    uint8_t response_status = get_response(1);
    if (response_status == COMM_TIMEOUT) {
#if DEBUG_PRINT
        Serial.println("received timeout");
#endif
        return COMM_TIMEOUT;
    } else if (response_status == COMM_CORRUPT_RESPONSE) {
#if DEBUG_PRINT
        Serial.println("corrupt response");
#endif
        return COMM_CORRUPT_RESPONSE;
    }
#if DEBUG_PRINT || TESTING_ORDER_OF_EVENTS
    Serial.print("response from infinity: ");
    Serial.println(infinity_response[1]);
    if (infinity_response[1] == COMM_SUCCESS) {
        Serial.println("Successfully got the stone in gauntlet!");
    }
#endif
    return infinity_response[1];
}

/**
 * Request put stone in gauntlet
 * Does not wait for reply
 */
uint8_t deploy_gauntlet() 
{
#if DEBUG_PRINT || TESTING_ORDER_OF_EVENTS
    Serial.println("Request gauntlet deployment");
#endif
    uint8_t send_status = send_command(DEPLOY_GAUNTLET);
    if (send_status == COMM_TIMEOUT) {
#if DEBUG_PRINT
        Serial.println("send timeout");
#endif
        return COMM_TIMEOUT;
    }

    uint8_t response_status = get_response(1);
    if (response_status == COMM_TIMEOUT) {
#if DEBUG_PRINT
        Serial.println("received timeout");
#endif
        return COMM_TIMEOUT;
    } else if (response_status == COMM_CORRUPT_RESPONSE) {
#if DEBUG_PRINT
        Serial.println("corrupt response");
#endif
        return COMM_CORRUPT_RESPONSE;
    }
#if DEBUG_PRINT || TESTING_ORDER_OF_EVENTS
    Serial.print("response from infinity: ");
    Serial.println(infinity_response[1]);
    if (infinity_response[1] == COMM_SUCCESS) {
        Serial.println("succesfully deployed gauntlet!");
    }
#endif
    return infinity_response[1];
}


/**
 * Requests grab infinity stone
 * See doc - RIGHT THE CONDITIONAL 3 try. Ignore for now, we'll see if that becomes necessary
 */
uint8_t grab_infinity_stone(byte side, byte post_num)
{
#if DEBUG_PRINT || TESTING_ORDER_OF_EVENTS
    Serial.println("grab_infinity_stone");
#endif
    if (side == LEFT) {
        side = 0x00;
    } else if (side == RIGHT) {
        side = 0xAA;
    }

    uint8_t send_status = send_command_get_stone_no_retries(side, post_num);

    if (send_status == COMM_TIMEOUT) {
#if DEBUG_PRINT
        Serial.println("send timeout");
#endif
        return COMM_TIMEOUT;
    }

    // get acknowledgment
    uint8_t response_status = get_response(1);
    if (response_status == COMM_TIMEOUT) {
#if DEBUG_PRINT
        Serial.println("receive timeout");
#endif
        return COMM_TIMEOUT;
    } else if (response_status == COMM_CORRUPT_RESPONSE) {
#if DEBUG_PRINT
        Serial.println("Corrupt response");
#endif
        return COMM_CORRUPT_RESPONSE;
    }
    if (infinity_response[1] == COMM_SUCCESS) {
#if DEBUG_PRINT
        Serial.println("I feel acknowledged, thanks <3");
#endif
    }

    //wait for response
    while (!CommSerial.available()) {
#if DEBUG_PRINT && !TESTING_ORDER_OF_EVENTS
        Serial.println("I pine for your response");
#endif
    } 

    // retrieve response
    response_status = get_response(1);
    if (response_status == COMM_TIMEOUT) {
#if DEBUG_PRINT
        Serial.println("receive timeout");
#endif
        return COMM_TIMEOUT;
    } else if (response_status == COMM_CORRUPT_RESPONSE) {
#if DEBUG_PRINT
        Serial.println("Corrupt response");
#endif
        return COMM_CORRUPT_RESPONSE;
    }
#if DEBUG_PRINT || TESTING_ORDER_OF_EVENTS
    Serial.print("response from infinity: ");
    Serial.println(infinity_response[1]);

    if (infinity_response[1] == COMM_SUCCESS) {
        Serial.println("successfully picked up stone!");
    }
#endif

    return infinity_response[1]; 
}

// HELPER __________________________________________________________________
uint8_t send_command(byte command) 
{
    int i;
    int sent = 0;
    for (i = 0; i < MAX_RETRIES_COMM; i++) {
        sent = send_command_no_retries(command);
#if DEBUG_PRINT
        Serial.print("success rate:");
        Serial.println(sent);
#endif
        if (sent == COMM_SUCCESS) {
            return COMM_SUCCESS;
        }
#if DEBUG_PRINT
        Serial.println("sending command timed out");
#endif
        delay(WAIT_BETWEEN_COMM_RETRIES);
    }

    if (sent != COMM_SUCCESS) {
        return COMM_TIMEOUT;
    }
    return COMM_SUCCESS;
}

uint8_t get_response(byte byte_num)
{
    int i;
    int received = 0;
    for (i = 0; i < MAX_RETRIES_COMM; i++) {
        received = get_response_no_retries(byte_num);
        if (received == COMM_SUCCESS) {
            return COMM_SUCCESS;
        }
#if DEBUG_PRINT
        Serial.println("receiving command timed out");
#endif
        delay(WAIT_BETWEEN_COMM_RETRIES);
    }
    if (received != COMM_SUCCESS) {
        return received;
    }
    return COMM_SUCCESS;
}

uint8_t send_command_no_retries(byte command) 
{    
    byte command_buff[3] = {START, command, STOP};

    uint32_t start_time = millis();

    while (!CommSerial.availableForWrite()) {
        if (millis() - start_time > MAX_TIMEOUT) {
            return COMM_TIMEOUT;
        }
    }
#if DEBUG_PRINT
    Serial.println("sending buffer contents: ");
    Serial.println(command_buff[0]);
    Serial.println(command_buff[1]);
    Serial.println(command_buff[2]);
#endif
    CommSerial.write(command_buff, 3);
    CommSerial.flush();
#if DEBUG_PRINT
    Serial.println("flushed");
#endif
    return COMM_SUCCESS;
}

uint8_t send_command_get_stone(byte side, byte post_num) 
{
    int i;
    int sent = 0;
    for (i = 0; i < MAX_RETRIES_COMM; i++) {
        sent = (send_command_get_stone_no_retries(side, post_num) == COMM_SUCCESS);
        if (sent) {
            break;
        }
#if DEBUG_PRINT
        Serial.println("sending command timed out");
#endif
        delay(WAIT_BETWEEN_COMM_RETRIES);
    }
    if (sent != COMM_SUCCESS) {
        return COMM_TIMEOUT;
    }
    return COMM_SUCCESS;
}

uint8_t send_command_get_stone_no_retries(byte side, byte post_num) 
{    
    byte command_buff[5] = {START, GRAB_STONE, side, post_num, STOP};

    uint32_t start_time = millis();

    while (!CommSerial.availableForWrite()) {
        if (millis() - start_time > MAX_TIMEOUT) {
            return COMM_TIMEOUT;
        }
    }
#if DEBUG_PRINT
    Serial.println("sending buffer contents: ");
    Serial.println(command_buff[0]);
    Serial.println(command_buff[1]);
    Serial.println(command_buff[2]);
    Serial.println(command_buff[3]);
    Serial.println(command_buff[4]);
#endif
    CommSerial.write(command_buff, 5);
    CommSerial.flush();
#if DEBUG_PRINT
    Serial.println("flushed");
#endif
    return COMM_SUCCESS;
}

uint8_t get_response_no_retries(uint8_t byte_num) 
{
    uint32_t start_time = millis();
    while (CommSerial.available() < byte_num + 2) {
        if (millis() - start_time > MAX_TIMEOUT) {
            return COMM_TIMEOUT;
        }
    }
    CommSerial.readBytes(infinity_response, 2 + byte_num);
    if (infinity_response[0] != START || infinity_response[2] != STOP) {
        return COMM_CORRUPT_RESPONSE;
    }

    return COMM_SUCCESS;
}