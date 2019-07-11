#include "AllPurposeInclude.h"
#include "StateControl/HandleCollision.h"

void handle_collision()
{   
    Serial.println("");
    Serial.println("");
    Serial.println("HANDLE_COLLISION state entered!");
    Serial.println("______________________");

    Serial.println("This state is not implemented yet. At all");

    //SET GLOBAL WHICH SIDE WE HIT VAR BACK TO NO_COLLISION
    if (digitalRead(MASTER_SWITCH) == DEV) {
        switch_state(HANDLE_COLLISION, MENU);
    } else {
        switch_state(HANDLE_COLLISION, REACH_RAMP);
    }
}