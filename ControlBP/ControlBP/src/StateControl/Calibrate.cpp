#include <Arduino.h>

#include "AllPurposeInclude.h"
#include "StateControl/Calibrate.h"

void calibrate()
{
    Serial.println("CALIBRATE state entered!");
    Serial.println("______________________");

    // Current location plan. Save wheel turns at each interlude
    // like that we'll be able to subtract out each time what we had at prev checkpoint
    // and know precisely what's changed since the last checkpoint
    bot_position.wheel_rot_since_last_left = request_wheel_rotations_left();
    bot_position.wheel_rot_since_last_right = request_wheel_rotations_right();

    bot_previous_state = CALIBRATE;
    if (digitalRead(MASTER_SWITCH) == COMP) {
        bot_state = FIND_POST;
    } else {
        bot_state = FIND_POST;
    }

}