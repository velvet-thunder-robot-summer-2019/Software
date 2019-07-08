#include "AllPurposeInclude.h"
#include "DecisionMaking/DecisionMaking.h"
#include "StateControl/FindPost.h"

int debug_iterations = 0;

void find_post()
{
    Serial.println("");
    Serial.println("");
    Serial.println("FIND_POST state entered!");
    Serial.println("______________________");

    // int post_num = pick_post_to_find();
    int side = get_checkpoint_expected_side();
    
    //OK... this will be more complicated, we'll have to either:
    //1. pick direction to follow tape in or
    //2. Go cross country
    //3. Decide which side of tape we expect post on - that'll return
    
    //code after this assumes we're on the tape facing
    //the direction we want
    if (robot_state() != FIND_POST) {
        return;
    }

    while (!branch_reached(side)) {
        uint8_t response = follow_tape();
        if (response == TAPE_NOT_FOUND) {
            backtrack_to_tape();
        }
        if (robot_state() != FIND_POST) {
            return;
        }
    }
    // UPDATE bot_position!!!!!

    if (align_to_branch() == STATE_CHANGED)
        return;
    
    int susan_angle = (side == LEFT) ? 0 : 360;

    if (request_susan_angle(susan_angle) == STATE_CHANGED)
        return;
    
    if (request_confirmation_post_presence(side) == STATE_CHANGED)
        return;
    
    debug_iterations++;
    Serial.print("debug iterations: ");
    Serial.println(debug_iterations);

    if (debug_iterations >= 2) {
        switch_state(FIND_POST, RETURN_TO_GAUNTLET);
    }
    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(FIND_POST, GET_INFINITY_STONE);
    } else {
        switch_state(FIND_POST, MENU);
    }
}
