#include <Arduino.h>

#include "Initialisation/GlobalVars.h"

/**
 * Doesn't wait for response bc we can prob just GO without return 
 * Requests for Infinity BP to position arm for travel
 */
void request_arm_position__travel()
{
    Serial.println("request_arm_position__travel");

}

/**
 * Requests for Infinity BP to position arm for ascent
 * Returns: Error code returned by Infinity BP, code TBD
 */
uint8_t request_arm_position__ascent()
{
    Serial.println("request_arm_position__ascent");
    return SUCCESS;
}

/**
 * Checks whether or not a U-turn has been performed yet (start of course)
 * Returns: True if U turn has been performed
 *          False otherwise
 */
uint8_t request_U_turn_status()
{
    Serial.println("request_U_turn_status");
    return SUCCESS;  
}

/**
 * CONSIDER RETURNING POINTER
 */
uint8_t request_recent_path()
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
 * 
 */
uint8_t request_confirmation_post_presence()
{
    Serial.print("request_confirmation_post_presence: ");
    Serial.println("POST FOUND! Except not really");
    return SUCCESS;
}