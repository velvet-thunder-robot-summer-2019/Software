#include "AllPurposeInclude.h"
#include "StateControl/ReturnToGauntlet.h"
#include "DecisionMaking/DecisionMaking.h"

void return_to_gauntlet()
{
    Serial.println("RETURN_TO_GAUNTLET state entered!");
    Serial.println("______________________");

    if (align_direction_to_return() == STATE_CHANGED)
    {
        return;
    }

    int gauntlet = (run_status.bot_identity == METHANOS) ? METHANOS_GAUNTLET : THANOS_GAUNTLET;
    int branch_side = get_checkpoint_expected_side();

    while (run_status.bot_position.last_location != gauntlet) {
        uint8_t response = follow_tape();
        if (response == TAPE_NOT_FOUND) {
            backtrack_to_tape();
        }
        if (robot_state() != RETURN_TO_GAUNTLET) {
            return;
        }
        if (branch_reached(branch_side)) {
            update_position();
        }

        //TODO: remove this stupid line
        run_status.bot_position.last_location = (run_status.bot_identity == METHANOS) ? METHANOS_GAUNTLET : THANOS_GAUNTLET;
    }

    if (align_to_gauntlet() != SUCCESS) {
        return;
    }
    

    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(RETURN_TO_GAUNTLET, FIT_TO_GAUNTLET);
    } else {
        switch_state(RETURN_TO_GAUNTLET, MENU);
    }
}