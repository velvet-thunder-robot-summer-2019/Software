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
        if (bot_state == RETURN_TO_GAUNTLET) {
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
    int next_slot_available = 0; // THIS IS JUST A STANDIN
    request_put_stone_in_gauntlet(next_slot_available);

    bot_previous_state = GET_INFINITY_STONE;
    if (digitalRead(MASTER_SWITCH) == COMP) {
        bot_state = FIND_POST;
    } else {
        bot_state = MENU;
    }
}