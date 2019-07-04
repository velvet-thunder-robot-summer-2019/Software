#include "AllPurposeInclude.h"
#include "StateControl/GoodnightSweetPrince.h"

void goodnight_sweet_prince()
{
    Serial.println("");
    Serial.println("");

    Serial.println("GOODNIGHT_SWEET_PRINCE state entered!");
    Serial.println("______________________");

    Serial.println("And flights of angels sing thee to thy rest");
    delay(10000); // TODO: change this to permanent delay 
    if (digitalRead(MASTER_SWITCH) == DEV) {
        bot_state = MENU;
    }
}