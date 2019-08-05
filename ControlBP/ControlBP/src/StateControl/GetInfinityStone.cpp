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
    switch_state(GET_INFINITY_STONE, FIND_POST);
    return;
#endif


    int side = get_branch_side(run_status.bot_position.next_location);

    // attempt to grab infinity stone
    int post_index = get_post_index();
    int result = grab_infinity_stone(side, post_index + 1);
    if (result == COMM_SUCCESS) {
        run_status.stones_status[post_index] = COLLECTED;
    } else if (result == COMM_TASK_FAILED) {
        run_status.stones_status[post_index] = MISSING;
    }
    
    result = request_put_stone_in_gauntlet();

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