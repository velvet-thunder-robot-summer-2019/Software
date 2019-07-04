#include "AllPurposeInclude.h"
#include "StateControl/FitToGauntlet.h"

void fit_to_gauntlet()
{
    Serial.println("");
    Serial.println("");
    Serial.println("FIT_TO_GAUNTLET state entered!");
    Serial.println("______________________");

    Serial.println("Julie figures we'll think of this later");
    
    bot_previous_state = FIT_TO_GAUNTLET;

    if (digitalRead(MASTER_SWITCH) == COMP) {
        bot_state = GOODNIGHT_SWEET_PRINCE;
    } else {
        bot_state = MENU;
    }
}