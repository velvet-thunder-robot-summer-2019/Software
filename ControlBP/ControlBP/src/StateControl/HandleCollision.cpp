#include "AllPurposeInclude.h"
#include "StateControl/HandleCollision.h"

#define BACKUP_TIME 1000

void handle_collision()
{   
#if DEBUG_PRINT
    Serial.println("");
    Serial.println("");
    Serial.println("HANDLE_COLLISION state entered!");
    Serial.println("______________________");

    Serial.println("This state is not implemented yet. At all");
#endif

    // uint32_t start_time = millis();
    // while (millis() - start_time < BACKUP_TIME) {
    //     reverse(FLAT_GROUND_TAPE_FOLLOWING_PWM);
    // }
    // stop_motors(BACK);

    // //SET GLOBAL WHICH SIDE WE HIT VAR BACK TO NO_COLLISION
    // if (digitalRead(MASTER_SWITCH) == DEV) {
    //     switch_state(HANDLE_COLLISION, MENU);
    // } else {
    //     switch_state(HANDLE_COLLISION, REACH_RAMP);
    // }
    // run_status.last_collision = NO_COLLISION;
}