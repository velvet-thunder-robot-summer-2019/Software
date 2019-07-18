#include <Arduino.h>

#include "Debugging/Menu.h"
#include "Debugging/Debug.h"
#include "Locomotion/PID.h"
#include "AllPurposeInclude.h"

#define MENU_REFRESH_DELAY 400 //ms, bring this down once display set up

typedef enum 
{
    CALIBRATION_MENU,
    DEBUG_MENU,
    STATE_MENU
} menu_page;

typedef enum
{
    CAL_TAPE_SENSOR,
    CAL_PID_PROP,
    CAL_PID_DERIV,
    CAL_EXIT   
}calibrate_menu;

typedef enum
{
    REACH_RAMP_MENU,
    ASCEND_RAMP_MENU,
    FIND_POST_MENU,
    GET_INFINITY_STONE_MENU,
    RETURN_TO_GAUNTLET_MENU,
    FIT_TO_GAUNTLET_MENU,
    HANDLE_COLLISION_MENU,
    GOODNIGHT_SWEET_PRINCE_MENU,
    EXIT_STATE_MENU
}state_selection_menu;

static menu_page menu_display;

void calibration_menu();
void state_menu();

void init_menu()
{
    pinMode(SET, INPUT);
    pinMode(NAVIGATE, INPUT);
}

void menu()
{
    Serial.println("Here be menu");
    /*
    Serial.print("NAVIGATE: ");
    Serial.println(digitalRead(NAVIGATE));
    Serial.print("SET: ");
    Serial.println(digitalRead(SET));
    */

    while(1) {
        switch (menu_display) {
            case CALIBRATION_MENU:
                Serial.println("Calibration menu");
                while (digitalRead(NAVIGATE) && digitalRead(SET)) {
                    if (digitalRead(MASTER_SWITCH) == COMP) {
                        Serial.println("Is comp, switching states");
                        switch_state(MENU, REACH_RAMP);
                        return;
                    }
                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    menu_display = DEBUG_MENU;
                } else if (!digitalRead(SET)) {
                    calibration_menu();
                }
                break;

            case DEBUG_MENU:
               Serial.println("Debug menu");
                while (digitalRead(NAVIGATE) && digitalRead(SET)) {
                    if (digitalRead(MASTER_SWITCH) == COMP) {
                        Serial.println("Is comp, switching states");
                        switch_state(MENU, REACH_RAMP);
                        return;
                    }
                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    menu_display = STATE_MENU;
                } else if (!digitalRead(SET)) {
                    debug();
                }
                break;
            case STATE_MENU:
                Serial.println("State menu");

                while (digitalRead(NAVIGATE) && digitalRead(SET)) {
                    if (digitalRead(MASTER_SWITCH) == COMP) {
                        Serial.println("Is comp, switching states");
                        switch_state(MENU, REACH_RAMP);
                        return;
                    }
                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    menu_display = CALIBRATION_MENU;
                } else if (!digitalRead(SET)) {
                    state_menu();
                }
                break;
        }
        if (robot_state() != MENU) {
            return;
        }
        delay(MENU_REFRESH_DELAY);
    }
}

void calibration_menu()
{
    calibrate_menu calibration_val = CAL_TAPE_SENSOR;
    int previous_calibration_value = analogRead(CALIBRATION_POTENTIOMETER);

    while (1) {
        delay(MENU_REFRESH_DELAY);

        switch (calibration_val) {
            case CAL_TAPE_SENSOR:
                Serial.print("Current tape sensor threshold: ");
                Serial.println(get_tape_sensor_threshold());
                Serial.print("Current potentiometer val: "); 
                Serial.println(analogRead(CALIBRATION_POTENTIOMETER));
                
                while (digitalRead(NAVIGATE) && digitalRead(SET)) {
                    if (digitalRead(MASTER_SWITCH) == COMP) {
                        switch_state(MENU, REACH_RAMP);
                        return;
                    }
                    int calibration_value = analogRead(CALIBRATION_POTENTIOMETER);

                    if (abs(previous_calibration_value - calibration_value) > CALIBRATION_DELTA_TO_PRINT) {
                        previous_calibration_value = calibration_value;
                        Serial.print("Current tape sensor threshold: ");
                        Serial.println(get_tape_sensor_threshold());
                        Serial.print("Current potentiometer val: "); 
                        Serial.println(calibration_value);
                    }

                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    calibration_val = CAL_PID_PROP;
                } else if (!digitalRead(SET)) {
                    update_threshold_tape_sensor();
                }
                break;

            case CAL_PID_PROP:
                Serial.print("Current kp: ");
                Serial.println(get_kp());
                Serial.print("Current potentiometer val: "); 
                Serial.println(analogRead(CALIBRATION_POTENTIOMETER));
                
                while (digitalRead(NAVIGATE) && digitalRead(SET)) {
                    if (digitalRead(MASTER_SWITCH) == COMP) {
                        switch_state(MENU, REACH_RAMP);
                        return;
                    }
                    int calibration_value = analogRead(CALIBRATION_POTENTIOMETER);

                    if (abs(previous_calibration_value - calibration_value) > CALIBRATION_DELTA_TO_PRINT) {
                        previous_calibration_value = calibration_value;

                        Serial.print("Current kp: ");
                        Serial.println(get_kp());
                        Serial.print("Current potentiometer val: "); 
                        Serial.println(calibration_value);
                    }

                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    calibration_val = CAL_PID_DERIV;
                } else if (!digitalRead(SET)) {
                    update_kp();
                }
                break;

            case CAL_PID_DERIV:
                Serial.print("Current kd: ");
                Serial.println(get_kd());
                Serial.print("Current potentiometer val: "); 
                Serial.println(analogRead(CALIBRATION_POTENTIOMETER));

                while (digitalRead(NAVIGATE) && digitalRead(SET)) {   
                    if (digitalRead(MASTER_SWITCH) == COMP) {
                        switch_state(MENU, REACH_RAMP);
                        return;
                    }
                    int calibration_value = analogRead(CALIBRATION_POTENTIOMETER);
                    
                    if (abs(previous_calibration_value - calibration_value) > CALIBRATION_DELTA_TO_PRINT) {
                        previous_calibration_value = calibration_value;
                        Serial.print("Current kd: ");
                        Serial.println(get_kd());
                        Serial.print("Current potentiometer val: "); 
                        Serial.println(calibration_value);
                    }                

                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    calibration_val = CAL_EXIT;
                } else if (!digitalRead(SET)) {
                    update_kd();
                }
                break;
            
            case CAL_EXIT:
                Serial.println("Exit");
                
                while (digitalRead(NAVIGATE) && digitalRead(SET)) {
                    if (digitalRead(MASTER_SWITCH) == COMP) {
                        switch_state(MENU, REACH_RAMP);
                        return;
                    }
                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    calibration_val = CAL_TAPE_SENSOR;
                } else if (!digitalRead(SET)) {
                    return;
                }
                break;
        }
        if (digitalRead(MASTER_SWITCH) == COMP) {
            switch_state(MENU, REACH_RAMP);
        }
        if (robot_state() != MENU) {
            Serial.println("going back to menu");
            return;
        }
    }
}

void state_menu()
{
    state_selection_menu displayed_state = REACH_RAMP_MENU;

    while (1) {
        delay(MENU_REFRESH_DELAY);

        switch (displayed_state) {
            case REACH_RAMP_MENU:
                Serial.println("reach ramp menu");
                while (digitalRead(NAVIGATE) && digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    displayed_state = ASCEND_RAMP_MENU;
                } else if (!digitalRead(SET)) {
                    switch_state(MENU, REACH_RAMP);
                    Serial.println("Entering reach ramp state");
                }
                break;
            
            case ASCEND_RAMP_MENU:
                Serial.println("ascend ramp menu");
                while (digitalRead(NAVIGATE) && digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    displayed_state = FIND_POST_MENU;
                } else if (!digitalRead(SET)) {
                    switch_state(MENU, ASCEND_RAMP);
                    Serial.println("Entering ascend ramp state");
                }
                break;
                
            case FIND_POST_MENU:
                Serial.println("find post menu");

                while (digitalRead(NAVIGATE) && digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    displayed_state = GET_INFINITY_STONE_MENU;
                } else if (!digitalRead(SET)) {
                    switch_state(MENU, REACH_RAMP);
                    Serial.println("Entering find post state");
                }
                break;
            
            case GET_INFINITY_STONE_MENU:
                Serial.println("get infinity stone menu");

                while (digitalRead(NAVIGATE) && digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    displayed_state = HANDLE_COLLISION_MENU;
                } else if (!digitalRead(SET)) {
                    switch_state(MENU, GET_INFINITY_STONE);
                    Serial.println("Entering get infinity stone state");
                }
                break;

            case HANDLE_COLLISION_MENU:
                Serial.println("handle collision menu");

                while (digitalRead(NAVIGATE) && digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    displayed_state = RETURN_TO_GAUNTLET_MENU;
                } else if (!digitalRead(SET)) {
                    switch_state(MENU, HANDLE_COLLISION);
                    Serial.println("Entering handle collision state");
                }
                break;

            case RETURN_TO_GAUNTLET_MENU:
                Serial.println("return to gauntlet menu");

                while (digitalRead(NAVIGATE) && digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    displayed_state = FIT_TO_GAUNTLET_MENU;
                } else if (!digitalRead(SET)) {
                    switch_state(MENU, RETURN_TO_GAUNTLET);
                    Serial.println("Entering return to gauntlet state");
                }
                break;

            case FIT_TO_GAUNTLET_MENU:
                Serial.println("fit to gauntlet menu");

                while (digitalRead(NAVIGATE) && digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    displayed_state = GOODNIGHT_SWEET_PRINCE_MENU;
                } else if (!digitalRead(SET)) {
                    switch_state(MENU, FIT_TO_GAUNTLET);
                    Serial.println("Entering fit to gauntlet state");
                }
                break;

            case GOODNIGHT_SWEET_PRINCE_MENU:
                Serial.println("goodnight sweet prince menu");
                while (digitalRead(NAVIGATE) && digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    displayed_state = EXIT_STATE_MENU;
                } else if (!digitalRead(SET)) {
                    switch_state(MENU, GOODNIGHT_SWEET_PRINCE);
                    Serial.println("Entering goodnight sweet prince state");
                }
                break;
            
            case EXIT_STATE_MENU:
            Serial.println("Exit");
                while (digitalRead(NAVIGATE) && digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    displayed_state = REACH_RAMP_MENU;
                } else if (!digitalRead(SET)) {
                    return;
                }
                break;
            }
        
        if (robot_state() != MENU) {
            return;
        }
    }
}