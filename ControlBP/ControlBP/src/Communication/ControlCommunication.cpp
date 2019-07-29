// TODO: add state checks while we wait in these methods that take a while!!

#include <Arduino.h>

#include "GlobalInfo/GlobalVars.h"
#include "GlobalInfo/HardwareDefs.h"
#include "Communication/ControlCommunication.h"

// Communication details
#define BUFFER_SIZE 7 // 2 for start/stop, 1 for return code, up to 4 for data
#define MAX_TIMEOUT 5000 //ms, max time to wait for availability
#define MAX_RETRIES_COMM 3 //max retries when a communication fails
#define WAIT_BETWEEN_COMM_RETRIES 5 

//Commands to Infinity 
//Arm commands
#define GET_ACK 0x00
#define GET_ARM_POSITION 0x03

#define SET_TRAVEL_POSITION 0x04
#define SET_ASCENT_POSITION 0x05
#define SET_STONE_IN_GAUNTLET 0x06
#define CONFIRM_POST_PRESENCE 0x07
#define ASCEND_POST 0x08
#define GRAB_STONE 0x09

// start, stop, return code
#define START 0xAA
#define STOP 0x55

//TODO: Gauntlet commands

uint8_t send_command(byte command);
uint8_t get_response(uint8_t byte_num);
uint8_t send_command_no_retries(byte command);
uint8_t get_response_no_retries(uint8_t byte_num);

HardwareSerial CommSerial = HardwareSerial(RX, TX);
byte infinity_response[BUFFER_SIZE];

/**
 * Initialises communication
 */
void init_communication()
{
    CommSerial.begin(115200);
    while (!CommSerial) {
        Serial.println("don't let the compiler optimise me away!!");
    }

    // Serial.println("init_communication");
}

/**
 * Tests communication, makes a simple check to see if we can speak to other BP and get response
 * Returns:     COMM_SUCCESS if communication established and we received ACK
 *              Various error messages otherwise
 */
uint8_t establish_communication()
{
    Serial.println("establish communication");
    uint8_t send_status = send_command(GET_ACK);
    if (send_status != COMM_SUCCESS) {
        return send_status;
    }

    uint8_t response_status = get_response(1);
    if (response_status == COMM_TIMEOUT) {
        return COMM_TIMEOUT;
    } else if (response_status == COMM_CORRUPT_RESPONSE) {
        Serial.println("corrupt response");
        return COMM_CORRUPT_RESPONSE;
    }
    if (infinity_response[1] == COMM_SUCCESS) {
        Serial.println("Initialised communication");
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
        Serial.println("send timeout");
        return COMM_TIMEOUT;
    }
    uint8_t response_status = get_response(4);
    if (response_status == COMM_TIMEOUT) {
        Serial.println("timeout");
        return COMM_TIMEOUT;
    } else if (response_status == COMM_CORRUPT_RESPONSE) {
        Serial.println("corrupt");
        return COMM_CORRUPT_RESPONSE;
    }

    if (infinity_response[1] != COMM_SUCCESS) {
        return infinity_response[1];
        Serial.print("arm angle obtained");
    }

    angles[0] = (uint8_t) infinity_response[2];
    angles[1] = (uint8_t) infinity_response[3];
    angles[2] = (uint8_t) infinity_response[4];
    angles[3] = (uint8_t) infinity_response[5];
    Serial.println("angle received");
    return COMM_SUCCESS;
}

/**
 * Doesn't wait for response, so we just check that command is acknowledged then move on
 * Requests for Infinity BP to position arm for travel
 */
uint8_t request_arm_position__travel()
{
    Serial.println("request_arm_position__travel");

    uint8_t send_status = send_command(SET_TRAVEL_POSITION);
    if (send_status == COMM_TIMEOUT) {
        Serial.println("send timeout");
        return COMM_TIMEOUT;
    }

    uint8_t response_status = get_response(1);
    if (response_status == COMM_TIMEOUT) {
        Serial.println("received timeout");
        return COMM_TIMEOUT;
    } else if (response_status == COMM_CORRUPT_RESPONSE) {
        Serial.println("corrupt response");
        return COMM_CORRUPT_RESPONSE;
    }
    Serial.print("response from infinity: ");
    Serial.println(infinity_response[1]);
    return infinity_response[1];
}

/**
 * Request put stone in gauntlet
 * Does not wait for reply
 */
uint8_t request_put_stone_in_gauntlet() 
{
    Serial.println("request_put_infinity_in_gauntlet: putting stone in gauntlet");
    uint8_t send_status = send_command(SET_STONE_IN_GAUNTLET);
    if (send_status == COMM_TIMEOUT) {
        Serial.println("send timeout");
        return COMM_TIMEOUT;
    }

    uint8_t response_status = get_response(1);
    if (response_status == COMM_TIMEOUT) {
        Serial.println("received timeout");
        return COMM_TIMEOUT;
    } else if (response_status == COMM_CORRUPT_RESPONSE) {
        Serial.println("corrupt response");
        return COMM_CORRUPT_RESPONSE;
    }
    Serial.print("response from infinity: ");
    Serial.println(infinity_response[1]);
    return infinity_response[1];
}

/**
 * Requests for Infinity BP to position arm for ascent
 * Returns: acknowledgement or error message (don't wait for response)
 */
uint8_t request_arm_position__ascent()
{
    // delay(1000);
    Serial.println("request_arm_position__ascent");
    uint8_t send_status = send_command(SET_ASCENT_POSITION);
    if (send_status == COMM_TIMEOUT) {
        Serial.println("send timeout");
        return COMM_TIMEOUT;
    }

    uint8_t response_status = get_response(1);
    if (response_status == COMM_TIMEOUT) {
        Serial.println("received timeout");
        return COMM_TIMEOUT;
    } else if (response_status == COMM_CORRUPT_RESPONSE) {
        Serial.println("corrupt response");
        return COMM_CORRUPT_RESPONSE;
    }
    Serial.print("response from infinity: ");
    Serial.println(infinity_response[1]);
    return infinity_response[1];
}

/**
 * Returns:     COMM_SUCCESS, COMM_TASK_FAILED, COMM_BUSY, COMM_CORRUPT_COMMAND
 */
uint8_t request_confirmation_post_presence(uint8_t side)
{
    Serial.print("request_confirmation_post_presence: ");
    uint8_t send_status = send_command(CONFIRM_POST_PRESENCE);
    if (send_status == COMM_TIMEOUT) {
        Serial.println("send timeout");
        return COMM_TIMEOUT;
    }

    while (!CommSerial.available()); // wait for response

    uint8_t response_status = get_response(1);
    if (response_status == COMM_TIMEOUT) {
        Serial.println("received timeout");
        return COMM_TIMEOUT;
    } else if (response_status == COMM_CORRUPT_RESPONSE) {
        Serial.println("corrupt response");
        return COMM_CORRUPT_RESPONSE;
    }
    Serial.print("response from infinity: ");
    Serial.println(infinity_response[1]);
    return infinity_response[1]; 
}

/**
 * Requests ascent of post (kind of climb up post, abstract this)
 * Internally checks if it's at the top
 * Returns:    COMM_SUCCESS, COMM_TASK_FAILED, COMM_BUSY, COMM_CORRUPT_COMMAND
 */
uint8_t request_post_ascent()
{
    Serial.println("Ascending post...");

    uint8_t send_status = send_command(ASCEND_POST);
    if (send_status == COMM_TIMEOUT) {
        Serial.println("send timeout");
        return COMM_TIMEOUT;
    }

    while (!CommSerial.available()); // wait for response

    uint8_t response_status = get_response(1);
    if (response_status == COMM_TIMEOUT) {
        Serial.println("received timeout");
        return COMM_TIMEOUT;
    } else if (response_status == COMM_CORRUPT_RESPONSE) {
        Serial.println("corrupt response");
        return COMM_CORRUPT_RESPONSE;
    }
    Serial.print("response from infinity: ");
    Serial.println(infinity_response[1]);
    return infinity_response[1]; 
}

/**
 * Requests grab infinity stone
 * See doc - RIGHT THE CONDITIONAL 3 try. Ignore for now, we'll see if that becomes necessary
 */
uint8_t grab_infinity_stone()
{
    Serial.println("grab_infinity_stone");

    uint8_t send_status = send_command(GRAB_STONE);
    if (send_status == COMM_TIMEOUT) {
        Serial.println("send timeout");
        return COMM_TIMEOUT;
    }

    while (!CommSerial.available()); // wait for response

    uint8_t response_status = get_response(1);
    if (response_status == COMM_TIMEOUT) {
        Serial.println("receive timeout");
        return COMM_TIMEOUT;
    } else if (response_status == COMM_CORRUPT_RESPONSE) {
        Serial.println("Corrupt response");
        return COMM_CORRUPT_RESPONSE;
    }
    Serial.print("response from infinity: ");
    Serial.println(infinity_response[1]);
    return infinity_response[1]; 
}

// HELPER __________________________________________________________________
uint8_t send_command(byte command) 
{
    int i;
    int sent = 0;
    for (i = 0; i < MAX_RETRIES_COMM; i++) {
        sent = (send_command_no_retries(command) == COMM_SUCCESS);
        if (sent) {
            break;
        }
        Serial.println("sending command timed out");
        delay(WAIT_BETWEEN_COMM_RETRIES);
    }
    if (sent != COMM_SUCCESS) {
        return COMM_TIMEOUT;
    }
    return COMM_SUCCESS;
}

uint8_t get_response(uint8_t byte_num)
{
    int i;
    int received = 0;
    for (i = 0; i < MAX_RETRIES_COMM; i++) {
        received = get_response_no_retries(byte_num);
        if (received == COMM_SUCCESS) {
            return COMM_SUCCESS;
        }
        Serial.println("receiving command timed out");
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
    Serial.println("sending buffer contents: ");
    Serial.println(command_buff[0]);
    Serial.println(command_buff[1]);
    Serial.println(command_buff[2]);
    CommSerial.write(command_buff, 3);
    CommSerial.flush();
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

// lower level commands, might not be needed ever _________________________________
/**
 * Request angle of lazy susan, ccw from standard axis
 * Params:      angle - in degrees
 * Returns:     SUCCESS - if successful
 *              STATE_CHANGED - if state changed
 */
// int request_susan_angle(int angle)
// {
//     Serial.print("request_susan_angle");
//     Serial.println(angle);
//     return 0;
// }


// /**
//  * Request angle of shoulder, from horizontal axis
//  * Params:      angle - in degrees
//  * Returns:     SUCCESS - if successful
//  *              STATE_CHANGED - if state changed
//  * */
// int request_shoulder_angle(int angle)
// {
//     Serial.print("request_shoulder_angle");
//     Serial.println(angle);
//     return 0;
// }

// /**
//  * Request angle of elbow 
//  * -90 is collinear to  base arm, +90 is extended opposite to base arm
//  * Params:      angle - in degrees
//  * Returns:     SUCCESS - if successful
//  *              STATE_CHANGED - if state changed
//  **/
// int request_elbow_angle(int angle)
// {
//     Serial.print("request_elbow_angle");
//     Serial.println(angle);
//     return 0;
// }

// /**
//  * Request angle of wrist 
//  * -90 is collinear to mid-arm, +90 is extended opposite to mid-arm
//  * Params:      angle - in degrees
//  * Returns:     angle set
//  */
// int request_wrist_angle(int angle)
// {
//     Serial.print("request_wrist_angle");
//     Serial.println(angle);
//     return 0;
// }

// /**
//  * Opens or closes claw
//  * Params:      position - OPEN or CLOSED
//  * Returns:     claw angle
//  */
// int request_claw_position(int position)
// {
//     Serial.print("request_claw_position");
//     if (position == OPEN) {
//         Serial.println("OPEN");
//     } else {
//         Serial.println("CLOSED");
//     }
//     return 0;
// }