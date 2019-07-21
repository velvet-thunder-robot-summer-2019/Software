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

/*
Example commands: init_tape_follower

 */

// source: https://startingelectronics.org/software/arduino/learn-to-program-course/19-serial-input/

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
        }

        delay(DELAY_BETWEEN_COMMANDS);
    }
}

//follow_tape wrapped for time period: 1 param time period, 1 param torque
