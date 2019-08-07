#include "AllPurposeInclude.h"
#include "StateControl/HandleCollision.h"

#define BACKUP_TIME 1500

int collision_retries = 0;
bool switch_branch = false;

void backup_at_collision();

void handle_collision()
{   
#if DEBUG_PRINT
    Serial.println("");
    Serial.println("");
    Serial.println("HANDLE_COLLISION state entered!");
    Serial.println("______________________");

    Serial.println("This state is not implemented yet. At all");
#endif
    if (collision_retries < 2) {
        backup_at_collision();
    } else {
        collision_retries = 0;
        if (we_have_stones()) {
            switch_state(HANDLE_COLLISION, RETURN_TO_GAUNTLET);
        } else {
            switch_branch = true;
            switch_state(HANDLE_COLLISION, FIND_POST);
        }
    }

    //SET GLOBAL WHICH SIDE WE HIT VAR BACK TO NO_COLLISION
    if (digitalRead(MASTER_SWITCH) == DEV) {
        switch_state(HANDLE_COLLISION, MENU);
    } else {
        switch_state(HANDLE_COLLISION, FIND_POST);
    }
}

void backup_at_collision() {
    uint32_t start_time = millis();
    while (millis() - start_time < BACKUP_TIME) {
        reverse(FLAT_GROUND_TAPE_FOLLOWING_PWM);
        get_tape_following_error();
    }
    stop_motors(BACK);
    collision_retries++;
}

bool we_have_stones()
{
    int i;
    for (i = 0; i < 6; i++) {
        if (run_status.stones_status[i] == COLLECTED) {
            return true;
        }
    }
    return false;
}

bool switch_branch_needed()
{
    return switch_branch;
}

void set_switch_branch_false()
{
    switch_branch = false;
}