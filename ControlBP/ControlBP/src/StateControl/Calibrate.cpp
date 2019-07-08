#include "AllPurposeInclude.h"
#include "StateControl/Calibrate.h"

void calibrate()
{
    Serial.println("CALIBRATE state entered!");
    Serial.println("______________________");

    // Current location plan. Save wheel turns at each interlude
    // like that we'll be able to subtract out each time what we had at prev checkpoint
    // and know precisely what's changed since the last checkpoint
    run_status.bot_position.wheel_rot_since_last_left = request_wheel_rotations_left();
    run_status.bot_position.wheel_rot_since_last_right = request_wheel_rotations_right();

    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(CALIBRATE, FIND_POST);
    } else {
        switch_state(CALIBRATE, MENU);
    }

}