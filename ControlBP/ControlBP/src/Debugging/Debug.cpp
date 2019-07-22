#include "Debugging/Debug.h"
#include <map>
#include <string>

// locomotion
#include "Locomotion/Locomotion.h"
#include "Locomotion/Motor.h"
#include "Locomotion/PID.h"
#include "Locomotion/TapeSensor.h"

#include "AllPurposeInclude.h"

//communication
//note: deal with those later since we don't have any of the code
#include "Communication/ControlCommunication.h"

#define DELAY_BETWEEN_COMMANDS 1000

void test_ramp_reached();
void test_reach_location_fwd();
void test_reach_location_back();

/*
Example commands: init_tape_follower

 */

// source: https://startingelectronics.org/software/arduino/learn-to-program-course/19-serial-input/

// navigation, suggested testing order (must be on tape unless specified)
/*
1. face reverse direction
2. reach location forwards
3. reach location backwards
4. follow arc 100 (no tape needed)
5. follow arc 50 (no tape needed)
6. turn onto branch (must place down bot in right spot first)
7. follow tape till branch
8. reach ramp state
9. ascend ramp state
10. find post state (assume you're at the intersection)
 */

void debug()
{
    Serial.println("Welcome to debug mode! Enter 'exit' to go back to menu. ");
    while (1) {

        int available = Serial.available();
        while (!available) {
            Serial.print("!available punk");
            available = Serial.available();
        }
        String command = Serial.readString();
        command.trim();

        Serial.print("Command received is: ");
        Serial.println(command);

        // can't use a switch statement bc that doesn't accept String types
        if (command.equals("exit")) {
            return;
        } else if (command.equals("init tape follower")) {
            init_tape_following();
        } else if (command.equals("align to branch")) {
            align_to_branch();
        } else if (command.equals("establish communication")) {
            establish_communication();
        } else if (command.equals("get arm angles")) {
            uint8_t angles[4];
            get_arm_angles(&angles[0]);
        } else if (command.equals("request arm position travel")) {
            request_arm_position__travel();
        } else if (command.equals("place stone in gauntlet")) {
            request_put_stone_in_gauntlet();
        } else if (command.equals("request arm position ascent")) {
            request_arm_position__ascent();
        } else if (command.equals("request confirmation post presence left")) {
            request_confirmation_post_presence(LEFT);
        } else if (command.equals("request confirmation post presence right")) {
            request_confirmation_post_presence(RIGHT);
        } else if (command.equals("request post ascent")) {
            request_post_ascent();
        } else if (command.equals("grab infinity stone")) {
            grab_infinity_stone();
        } else if (command.equals("test ramp_reached")) {
            test_ramp_reached();
        } else if (command.equals("reach location forwards")) {
            test_reach_location_fwd();
        } else if (command.equals("reach location backwards")) {
            test_reach_location_back();
        } else if (command.equals("turn onto branch")) {

        } else if (command.equals("follow arc 100")) {
            follow_arc_rho(LEFT, 100, TURN_PWM);
        } else if (command.equals("follow arc 50")) {
            follow_arc_rho(RIGHT, 50, TURN_PWM);
        } else if (command.equals("follow tape till branch")) {
            follow_tape_till_branch(MENU);
        } else if (command.equals("get_arm_angles")) {
            uint8_t arm_angles[4];
            get_arm_angles(&arm_angles[0]);
        }

        delay(DELAY_BETWEEN_COMMANDS);
    }
}

//follow_tape wrapped for time period: 1 param time period, 1 param torque
void test_ramp_reached() 
{
    delay(3000); // delays 3 seconds
    digitalWrite(BLINKY, HIGH); // turn off light
    while (!ramp_reached()) {
        follow_tape(FLAT_GROUND_TAPE_FOLLOWING_PWM);
    }
    digitalWrite(BLINKY, LOW);
}

void test_reach_location_fwd()
{
    Serial.println("put robot between POST_4 and POST_3, facing POST_3");
    run_status.bot_position.last_location = POST_4;
    run_status.bot_position.next_location = POST_3;
    delay(5000); // waits 5 seconds for you to unplug stuff
    reach_adjacent_location_on_tape(POST_3, MENU, true);
}

void test_reach_location_back()
{
    Serial.println("put robot between POST_4 and POST_3, facing POST_4");
    run_status.bot_position.last_location = POST_3;
    run_status.bot_position.next_location = POST_4;
    delay(5000); // waits 5 seconds for you to unplug stuff
    reach_adjacent_location_on_tape(POST_3, MENU, true);
}

void test_turn_onto_branch()
{
    Serial.println("place robot at intersection");
    delay(3000); // 3 seconds
    turn_onto_branch(LEFT, MENU);
}