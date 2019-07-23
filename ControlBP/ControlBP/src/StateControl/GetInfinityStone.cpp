#include "AllPurposeInclude.h"
#include "StateControl/GetInfinityStone.h"
#include "Debugging/Menu.h"

#define MAX_ATTEMPTS_STONE 3

//TODO: create array for different gauntlet slots so we know WHERE WE"VE PUT STONES ALREADY
//Place gauntlet
int get_post_index();

void get_infinity_stone()
{
#if TESTING_ORDER_OF_EVENTS || DEBUG_PRINT
    Serial.println("");
    Serial.println("");
    Serial.println("GET_INFINITY_STONE state entered!");
    Serial.println("______________________");
#endif
#if NO_ARM_TESTING
    stub_arm_motion();
    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(GET_INFINITY_STONE, FIND_POST);
    } else {
        switch_state(GET_INFINITY_STONE, MENU);
    }
    return;
#endif

    int side = run_status.bot_identity == THANOS ? LEFT : RIGHT;

    int confirm_post = request_confirmation_post_presence(side);
    if (confirm_post == STATE_CHANGED) {
        return;
    } else if (confirm_post == COMM_TASK_FAILED) {
        if (digitalRead(MASTER_SWITCH) == COMP) {
            switch_state(GET_INFINITY_STONE, FIND_POST);
        } else {
            switch_state(GET_INFINITY_STONE, MENU);
        }
    }
    int post_ascended = request_post_ascent();
    if (post_ascended == STATE_CHANGED) {
        return;
    } else if (post_ascended == COMM_TASK_FAILED) {
        if (digitalRead(MASTER_SWITCH) == COMP) {
            switch_state(GET_INFINITY_STONE, FIND_POST);
        } else {
            switch_state(GET_INFINITY_STONE, MENU);
        }
    }

    // attempt to grab infinity stones
    int i;
    int result;
    for (i = 0; i < MAX_ATTEMPTS_STONE; i++) {
        result = grab_infinity_stone();
        if (result == SUCCESS) {
            break;
        }
        if (robot_state() == RETURN_TO_GAUNTLET) {
            break;
        }
    }

    if (result == SUCCESS) {
        // for whichever post we're at, set corresponding stone status to COLLECTED
#if DEBUG_PRINT
        Serial.println("getting stone was a success!");
#endif
    } else {
        // for whichever post we're at, set corresponding stone status to MISSING
        Serial.println("Stone was missing");
    }
    int stone_in_gauntlet = request_put_stone_in_gauntlet();
    int post_index = get_post_index();
    if (stone_in_gauntlet == COMM_SUCCESS) {
        run_status.stones_status[post_index] = COLLECTED;
    } else if (stone_in_gauntlet == COMM_TASK_FAILED) {
        run_status.stones_status[post_index] = MISSING;
    }

    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(GET_INFINITY_STONE, FIND_POST);
    } else {
        switch_state(GET_INFINITY_STONE, MENU);
    }
}

int get_post_index() {
    switch(run_status.bot_position.last_location) {
        case POST_1:
            return 0;
        case POST_2:
            return 1;
        case POST_3:
            return 2;
        case POST_4:
            return 3;
        case POST_5:
            return 4;
        case POST_6:
            return 5;
        default:
            return -1;
    }
}