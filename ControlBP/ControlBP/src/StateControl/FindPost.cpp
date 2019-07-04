#include <Arduino.h>

#include "AllPurposeInclude.h"
#include "DecisionMaking/DecisionMaking.h"
#include "StateControl/FindPost.h"

void find_post()
{
    Serial.println("CALIBRATE state entered!");
    Serial.println("______________________");

    int side = pick_post_to_find();
    //OK... this will be more complicated, we'll have to either:
    //1. pick direction to follow tape in or
    //2. Go cross country
    //3. Decide which side of tape we expect post on - that'll return
    
    //code after this assumes we're on the tape facing
    //the direction we want
    if (bot_state != FIND_POST) {
        return;
    }

    while (!branch_reached(side)) {
        uint8_t response = follow_tape();
        if (response == TAPE_NOT_FOUND) {
            backtrack_to_tape();
        }
        if (bot_state != FIND_POST) {
            return;
        }
    }

    if (align_to_branch() == STATE_CHANGED)
        return;
    
    int susan_angle = (side == LEFT ? 0 : 360);

    if (request_susan_angle(susan_angle) == STATE_CHANGED)
        return;
    
    if (request_confirmation_post_presence(side) == STATE_CHANGED)
        return;
    
    bot_previous_state = FIND_POST;
    if (digitalRead(MASTER_SWITCH) == COMP) {
        bot_state = GET_INFINITY_STONE;
    } else {
        bot_state = MENU;
    }
    
}
