#include "AllPurposeInclude.h"
#include "StateControl/GetInfinityStone.h"

#define MAX_ATTEMPTS_STONE 3

//TODO: create array for different gauntlet slots so we know WHERE WE"VE PUT STONES ALREADY
//Place gauntlet

void get_infinity_stone()
{
    Serial.println("");
    Serial.println("");
    Serial.println("GET_INFINITY_STONE state entered!");
    Serial.println("______________________");

    request_post_ascent();

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
        Serial.println("getting stone was a success!");
    } else {
        // for whichever post we're at, set corresponding stone status to MISSING
        Serial.println("Stone was missing");
    }
    request_put_stone_in_gauntlet();

    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(GET_INFINITY_STONE, FIND_POST);
    } else {
        switch_state(GET_INFINITY_STONE, MENU);
    }
}