#include "AllPurposeInclude.h"
#include "StateControl/FitToGauntlet.h"

void fit_to_gauntlet()
{
#if DEBUG_PRINT
    Serial.println("");
    Serial.println("");
    Serial.println("FIT_TO_GAUNTLET state entered!");
    Serial.println("______________________");

    Serial.println("Julie figures we'll think of this later");
#endif

    if (digitalRead(MASTER_SWITCH) == COMP) {
        switch_state(FIT_TO_GAUNTLET, GOODNIGHT_SWEET_PRINCE);
    } else {
        switch_state(FIT_TO_GAUNTLET, MENU);
    }
}