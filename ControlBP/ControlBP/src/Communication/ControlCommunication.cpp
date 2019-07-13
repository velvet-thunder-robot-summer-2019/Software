#include <Arduino.h>

#include "GlobalInfo/GlobalVars.h"
#include "GlobalInfo/HardwareDefs.h"
#include "Communication/ControlCommunication.h"

// Communication details
#define BUFFER_SIZE 5 // 1 for return code, 4 for data
#define MAX_TIMEOUT 2 //ms, max time to wait for availability
#define MAX_RETRIES_COMM 3 //max retries when a communication fails
#define WAIT_BETWEEN_COMM_RETRIES 5 

//Commands to Infinity 
//Arm commands
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

//TODO: Gauntlet commands

int send_command(byte command);
int get_response(int byte_num);
int send_command_no_retries(byte command);
int get_response_no_retries(int byte_num);

HardwareSerial CommSerial = HardwareSerial(RX, TX);
byte infinity_response[BUFFER_SIZE];

/**
 * Initialises communication
 */
void init_communication()
{
    CommSerial.begin(115200);
    Serial.println("init_communication");
}

/**
 * Tests communication, makes a simple check to see if we can speak to other BP and get response
 * Returns:     TRUE if communication established and we received ACK
 *              FALSE otherwise
 */
int communication_established()
{
    if (send_command(GET_ACK) == TIMEOUT) {
        return TIMEOUT;
    }
    if (get_response(1) == TIMEOUT) {
        return TIMEOUT;
    }

    if (infinity_response[0] == ACK) {
        Serial.println("Initialised communication");
        return TRUE;
    }
    return CORRUPT_RESPONSE;
}

// request info ______________________________________________________________________
/**
 * Checks whether or not a U-turn has been performed yet (start of course)
 * Returns: True if U turn has been performed
 *          False otherwise
 */
int request_U_turn_status()
{
    // Serial.println("request_U_turn_status");
    /* 
    if (send_command(GET_ACK) == TIMEOUT) {
        return TIMEOUT;
    }
    if (get_response(2) == TIMEOUT) {
        return TIMEOUT;
    }
    if (infinity_response[0] != ACK) {
        Serial.println("Response incorrect");
        return CORRUPT_RESPONSE;
    } 
    return (int) infinity_response[1];
    */
   return FALSE;
}

/**
 * CONSIDER RETURNING POINTER
 */
int request_recent_path()
{
    Serial.println("request_recent_path");
    return SUCCESS;
}

/**
 * Returns:     number of left wheel rotations
 */
int request_wheel_rotations_left()
{
    Serial.println("request_wheel_rotation_left");
    if (send_command(GET_LEFT_WHEEL_ROT) == TIMEOUT) {
        return TIMEOUT;
    }
    if (get_response(3) == TIMEOUT) {
        return TIMEOUT;
    }
    if (infinity_response[0] != ACK) {
        return CORRUPT_RESPONSE;
    }
    Serial.print("ok so the byte breakdown is: ");
    Serial.println((byte)infinity_response[1]);
    Serial.println((byte)infinity_response[2]);
    Serial.println((int) (infinity_response[1] << 8 | infinity_response[2]));

    int rotations = (int) (infinity_response[1] << 8 | infinity_response[2]);
    return rotations;
}

// request motion __________________________________________________________________________

/**
 * CONSIDER returning a struct with left and right wheel info (2 ints)
 * Returns:     number of right wheel rotations
 */
int request_wheel_rotations_right()
{
    Serial.println("request_wheel_rotation_right");
    if (send_command(GET_RIGHT_WHEEL_ROT) == TIMEOUT) {
        return TIMEOUT;
    }
    if (get_response(3) == TIMEOUT) {
        return TIMEOUT;
    }
    if (infinity_response[0] != ACK) {
        return CORRUPT_RESPONSE;
    }
    Serial.print("ok so the byte breakdown is: ");
    Serial.println((byte)infinity_response[1]);
    Serial.println((byte)infinity_response[2]);
    Serial.println((int) (infinity_response[1] << 8 | infinity_response[2]));

    int rotations = (int) (infinity_response[1] << 8 | infinity_response[2]);
    return rotations;
}

/**
 * Doesn't wait for response, so we just check that command is acknowledged then move on
 * Requests for Infinity BP to position arm for travel
 */
int request_arm_position__travel()
{
    Serial.println("request_arm_position__travel");
    if (send_command(SET_TRAVEL_POSITION) == TIMEOUT) {
        return TIMEOUT;
    }
    if (get_response(1) == TIMEOUT) {
        return TIMEOUT;
    }
    if (infinity_response[0] != ACK) {
        return CORRUPT_RESPONSE;
    }
    return ACK;
}

/**
 * Request put stone in gauntlet
 * Does not wait for reply
 */
int request_put_stone_in_gauntlet(int next_slot_available) 
{
    Serial.println("request_put_infinity_in_gauntlet: putting stone in gauntlet");
    if (send_command(SET_STONE_IN_GAUNTLET) == TIMEOUT) {
        return TIMEOUT;
    }
    if (get_response(1) == TIMEOUT) {
        return TIMEOUT;
    }
    if (infinity_response[0] != ACK) {
        return CORRUPT_RESPONSE;
    }
    return ACK;
}

/**
 * Requests for Infinity BP to position arm for ascent
 * Returns: Error code returned by Infinity BP, code TBD
 */
int request_arm_position__ascent()
{
    delay(1000);
    Serial.println("request_arm_position__ascent");
    if (send_command(SET_ASCENT_POSITION) == TIMEOUT) {
        return TIMEOUT;
    }
    if (get_response(1) == TIMEOUT) {
        return TIMEOUT;
    }
    if (infinity_response[0] != ACK) {
        return CORRUPT_RESPONSE;
    }
    return ACK;
}

/**
 * Returns:     ACK - if all went smoothly, post found
 *              FAIL - if post not found
 *              BUSY - if last command not finished
 *              NO_RESPONSE - if we're ghosted
 */
int request_confirmation_post_presence(int side)
{
    Serial.print("request_confirmation_post_presence: ");
    if (send_command(CONFIRM_POST_PRESENCE) == TIMEOUT) {
        return TIMEOUT;
    }
    if (get_response(1) == TIMEOUT) {
        return TIMEOUT;
    }
    delay(1000);
    Serial.println("POST FOUND! Except not really");
    return (int) infinity_response[0]; 
}

/**
 * Requests ascent of post (kind of climb up post, abstract this)
 * Internally checks if it's at the top
 * Returns:     ACK - if all went smoothly, post found
 *              FAIL - if post not found
 *              BUSY - if last command not finished
 */
int request_post_ascent()
{
    Serial.println("Ascending post...");
    if (send_command(SET_ASCENT_POSITION) == TIMEOUT) {
        return TIMEOUT;
    }
    if (get_response(1) == TIMEOUT) {
        return TIMEOUT;
    }
    delay(5000);
    return infinity_response[0];
}

/**
 * Requests grab infinity stone
 * See doc - RIGHT THE CONDITIONAL 3 try. Ignore for now, we'll see if that becomes necessary
 */
int grab_infinity_stone()
{
    Serial.println("grab_infinity_stone... stone grabbed!");
    if (send_command(SET_ASCENT_POSITION) == TIMEOUT) {
        return TIMEOUT;
    }
    if (get_response(1) == TIMEOUT) {
        return TIMEOUT;
    }
    delay(2000);
    return infinity_response[0];
}

// HELPER __________________________________________________________________
int send_command(byte command) 
{
    int i;
    int sent = 0;
    for (i = 0; i < MAX_RETRIES_COMM; i++) {
        sent = (send_command_no_retries(command) == SUCCESS);
        if (sent) {
            break;
        }
        Serial.println("sending command timed out");
        delay(WAIT_BETWEEN_COMM_RETRIES);
    }
    if (sent != SUCCESS) {
        return TIMEOUT;
    }
    return SUCCESS;
}

int get_response(int byte_num)
{
    int i;
    int received = 0;
    for (i = 0; i < MAX_RETRIES_COMM; i++) {
        received = get_response_no_retries(byte_num);
        if (received == SUCCESS) {
            return SUCCESS;
        }
        Serial.println("receiving command timed out");
        delay(WAIT_BETWEEN_COMM_RETRIES);
    }
    if (received != SUCCESS) {
        return TIMEOUT;
    }
    return SUCCESS;
}

int send_command_no_retries(byte command) 
{    
    uint32_t start_time = millis();

    while (!CommSerial.availableForWrite()) {
        if (millis() - start_time > MAX_TIMEOUT) {
            return TIMEOUT;
        }
    }
    CommSerial.print(command);
    return SUCCESS;
}

int get_response_no_retries(int byte_num) 
{
    uint32_t start_time = millis();
    while (!CommSerial.available()) {
        if (millis() - start_time > MAX_TIMEOUT) {
            return TIMEOUT;
        }
    }
    CommSerial.readBytes(infinity_response, byte_num);
    return SUCCESS;
}

// lower level commands, might not be needed ever _________________________________
/**
 * Request angle of lazy susan, ccw from standard axis
 * Params:      angle - in degrees
 * Returns:     SUCCESS - if successful
 *              STATE_CHANGED - if state changed
 */
int request_susan_angle(int angle)
{
    Serial.print("request_susan_angle");
    Serial.println(angle);
    return 0;
}


/**
 * Request angle of shoulder, from horizontal axis
 * Params:      angle - in degrees
 * Returns:     SUCCESS - if successful
 *              STATE_CHANGED - if state changed
 * */
int request_shoulder_angle(int angle)
{
    Serial.print("request_shoulder_angle");
    Serial.println(angle);
    return 0;
}

/**
 * Request angle of elbow 
 * -90 is collinear to  base arm, +90 is extended opposite to base arm
 * Params:      angle - in degrees
 * Returns:     SUCCESS - if successful
 *              STATE_CHANGED - if state changed
 **/
int request_elbow_angle(int angle)
{
    Serial.print("request_elbow_angle");
    Serial.println(angle);
    return 0;
}

/**
 * Request angle of wrist 
 * -90 is collinear to mid-arm, +90 is extended opposite to mid-arm
 * Params:      angle - in degrees
 * Returns:     angle set
 */
int request_wrist_angle(int angle)
{
    Serial.print("request_wrist_angle");
    Serial.println(angle);
    return 0;
}

/**
 * Opens or closes claw
 * Params:      position - OPEN or CLOSED
 * Returns:     claw angle
 */
int request_claw_position(int position)
{
    Serial.print("request_claw_position");
    if (position == OPEN) {
        Serial.println("OPEN");
    } else {
        Serial.println("CLOSED");
    }
    return 0;
}