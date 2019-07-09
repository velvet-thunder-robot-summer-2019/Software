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
Example commands:

 */

// source: https://startingelectronics.org/software/arduino/learn-to-program-course/19-serial-input/

void debug()
{
    Serial.println("Welcome to debug mode! Enter 'exit' to go back to menu. ");
    while (1) {

        int available = Serial.available();
        while (!available) {
            available = Serial.available();
        }
        String command = Serial.readString();
        command.trim();

        Serial.print("Command received is: ");
        Serial.println(command);

        // can't use a switch statement bc that doesn't accept String types
        if (command.equals("exit")) {
            return;
        } else if (command.equals("init_tape_follower")) {
            init_tape_following();
        } else if (command.equals("align_to_branch")) {
            align_to_branch();
        } 

        delay(DELAY_BETWEEN_COMMANDS);
    }
}

//follow_tape wrapped for time period: 1 param time period, 1 param torque
