#include "Debugging/Debug.h"
#include <map>
#include <string>

// locomotion
#include "Locomotion/Locomotion.h"
#include "Locomotion/Motor.h"
#include "Locomotion/PID.h"
#include "Locomotion/TapeSensor.h"

#include "GlobalInfo/Interrupts.h"

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
- Results: have face reverse direction update tape sensors so that it fixes itself w/ tape sensor
2. reach location forwards
- Results: make it stop earlier, then back up
3. reach location backwards
- Ditto to 2.
4. follow arc 100 (no tape needed)
- pretty shallow, not the best choice
5. follow arc 50 (no tape needed)
- better angle
6. follow tape till branch
7. turn onto branch (must place down bot in right spot first)
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
            // Serial.print("!available punk");
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
        } else if (command.equals("turn onto branch right")) {
            delay(1000);
            turn_onto_branch(RIGHT, MENU);
        } else if (command.equals("follow arc 25")) {
            uint32_t start_time = millis();
            while (millis() - start_time < 10000) {
                follow_arc_rho(LEFT, 25, TURN_PWM);
            }
        } else if (command.equals("follow arc 45")) {
            uint32_t start_time = millis();
            while (millis() - start_time < 10000) {
                follow_arc_rho(LEFT, 45, TURN_PWM);
            }      
        } else if (command.equals("follow tape till branch")) {
            follow_tape_till_branch(MENU);
        } else if (command.equals("get_arm_angles")) {
            uint8_t arm_angles[4];
            get_arm_angles(&arm_angles[0]);
        } else if (command.equals("face reverse direction")) {
            delay(3000);
            face_reverse_direction(MENU);
        } else if (command.equals("get all tape sensors")) {
            Serial.println();
            Serial.print("inside left: ");
            Serial.println(left_sensor());
            Serial.println(analogRead(LEFT_SENSOR));

            Serial.print("inside right: ");
            Serial.println(right_sensor());
            Serial.println(analogRead(RIGHT_SENSOR));

            Serial.print("outside left: ");
            Serial.println(outer_left_sensor());
            Serial.println(analogRead(OUTER_LEFT_SENSOR));

            Serial.print("outside right: ");
            Serial.println(outer_right_sensor());
            Serial.println(analogRead(OUTER_RIGHT_SENSOR));

            // Serial.print("wing left: ");
            // Serial.println(left_wing_sensor());
            // Serial.println(analogRead(LEFT_WING_SENSOR));

            // Serial.print("wing right: ");
            // Serial.println(right_wing_sensor());
            // Serial.println(analogRead(RIGHT_WING_SENSOR));
        } else if (command.equals("follow tape")) {
            uint32_t start_time = millis();
            while (millis() - start_time < 30000) {
                follow_tape(FLAT_GROUND_TAPE_FOLLOWING_PWM);
            }
            stop_motors();
        } else if (command.equals("get last click speed")) {
            uint32_t avg_velocity_left = (left_wheel_dt[0] + left_wheel_dt[1] + left_wheel_dt[2]) / 3;
            uint32_t avg_velocity_right = (right_wheel_dt[0] + right_wheel_dt[1] + right_wheel_dt[2]) / 3;
            Serial.println("ramp_reached method");
            Serial.print("avg velocity left: ");
            Serial.println(avg_velocity_left);
            Serial.print("avg velocity right");
            Serial.println(avg_velocity_right);
        } else if (command.equals("run straight")) {
            while (true) {
                run_motor(LEFT, FWD, 0.4);
                run_motor(RIGHT, FWD, 0.4);
            }
        } else if (command.equals("run left")) {
            while (true) {
                run_motor(LEFT, FWD, 0.4);
            }
        } else if (command.equals("blinky")) {
            digitalWrite(BLINKY, HIGH);
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