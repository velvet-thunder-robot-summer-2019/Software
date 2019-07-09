#include <Arduino.h>

#include "GlobalInfo/GlobalVars.h"
#include "GlobalInfo/HardwareDefs.h"


//Commands to Infinity
#define GET_ACK 0x0
#define GET_U_TURN_STATUS 0x1
#define GET_LEFT_WHEEL_ROT 0x2
#define GET_RIGHT_WHEEL_ROT 0x3

#define SET_TRAVEL_POSITION 0x4
#define SET_TRAVEL_ASCENT 0x5 





void init_communication()
{
    Serial.println("Initialised communication");
    // send GET_ACK

}

// request info
/**
 * Checks whether or not a U-turn has been performed yet (start of course)
 * Returns: True if U turn has been performed
 *          False otherwise
 */
int request_U_turn_status()
{
    Serial.println("request_U_turn_status");
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
    return SUCCESS;
}

//request motion

/**
 * CONSIDER returning a struct with left and right wheel info (2 ints)
 * Returns:     number of right wheel rotations
 */
int request_wheel_rotations_right()
{
    Serial.println("request_wheel_rotation_right");
    return SUCCESS;
}

/**
 * Doesn't wait for response bc we can prob just GO without return 
 * Requests for Infinity BP to position arm for travel
 */
void request_arm_position__travel()
{
    Serial.println("request_arm_position__travel");
}

/**
 * Request put stone in gauntlet
 * Does not wait for reply
 */
void request_put_stone_in_gauntlet(int next_slot_available) 
{
    Serial.println("request_put_infinity_in_gauntlet: putting stone in gauntlet");
}

/**
 * Requests for Infinity BP to position arm for ascent
 * Returns: Error code returned by Infinity BP, code TBD
 */
int request_arm_position__ascent()
{
    delay(1000);
    Serial.println("request_arm_position__ascent");
    return SUCCESS;
}

/**
 * 
 */
int request_confirmation_post_presence(int side)
{
    Serial.print("request_confirmation_post_presence: ");
    delay(1000);
    Serial.println("POST FOUND! Except not really");
    return SUCCESS;
}

/**
 * Requests ascent of post (kind of climb up post, abstract this)
 * Internally checks if it's at the top
 * Returns:     SUCCESS - if it detects top of post
 *              ERROR   - if something goes horribly wrong
 */
int request_post_ascent()
{
    Serial.println("Ascending post...");
    delay(5000);
    return SUCCESS;
}

/**
 * Requests grab infinity stone
 * See doc
 */
int grab_infinity_stone()
{
    delay(2000);
    Serial.println("grab_infinity_stone... stone grabbed!");
    return SUCCESS;
}

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