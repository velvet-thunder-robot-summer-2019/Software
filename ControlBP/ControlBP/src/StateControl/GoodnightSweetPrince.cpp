#include "AllPurposeInclude.h"
#include "StateControl/GoodnightSweetPrince.h"

void goodnight_sweet_prince()
{
#if DEBUG_PRINT || TESTING_ORDER_OF_EVENTS
    Serial.println("");
    Serial.println("");

    Serial.println("GOODNIGHT_SWEET_PRINCE state entered!");
    Serial.println("______________________");

    Serial.println("And flights of angels sing thee to thy rest");
#endif
    deploy_gauntlet();
    delay(1000000); // TODO: change this to permanent delay

    if (digitalRead(MASTER_SWITCH) == DEV) {
        switch_state(GOODNIGHT_SWEET_PRINCE, MENU);
    }
}