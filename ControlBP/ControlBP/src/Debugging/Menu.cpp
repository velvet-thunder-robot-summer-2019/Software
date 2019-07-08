#include <Arduino.h>

#include "Debugging/Menu.h"
#include "Locomotion/PID.h"
#include "AllPurposeInclude.h"

#define MENU_REFRESH_DELAY 1000 //ms, bring this down once display set up

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
    CALIBRATE_MENU,
    FIND_POST_MENU,
    GET_INFINITY_STONE_MENU,
    RETURN_TO_GAUNTLET_MENU,
    FIT_TO_GAUNTLET_MENU,
    HANDLE_COLLISION_MENU,
    GOODNIGHT_SWEET_PRINCE_MENU
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
    while(1) {
        switch (menu_display) {
            case CALIBRATION_MENU:
                Serial.println("Calibration menu");
                calibration_menu();
                break;
            case DEBUG_MENU:
                Serial.println("debug menu");
                switch_state(MENU, DEBUG);
                break;
            case STATE_MENU:
                Serial.println("state menu");
                state_menu();
                break;
        }
        if (robot_state() != MENU) {
            return;
        }
    }
}

void calibration_menu()
{
    while (1) {
        calibrate_menu calibration_val = CAL_TAPE_SENSOR;
        delay(MENU_REFRESH_DELAY);

        switch (calibration_val) {
            case CAL_TAPE_SENSOR:
                while (!digitalRead(NAVIGATE) && !digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY);
                    
                    Serial.print("Current tape sensor threshold: ");
                    Serial.println(get_tape_sensor_threshold());
                    Serial.print("Current potentiometer val: "); 
                    Serial.println(analogRead(CALIBRATION_POTENTIOMETER));
                }
                if (digitalRead(NAVIGATE)) {
                    calibration_val = CAL_PID_PROP;
                } else if (digitalRead(SET)) {
                    update_threshold_tape_sensor();
                }
                break;

            case CAL_PID_PROP:
                while (!digitalRead(NAVIGATE) && !digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY);

                    Serial.print("Current kp: ");
                    Serial.println(get_kp());
                    Serial.print("Current potentiometer val: "); 
                    Serial.println(analogRead(CALIBRATION_POTENTIOMETER));
                }
                if (digitalRead(NAVIGATE)) {
                    calibration_val = CAL_PID_DERIV;
                } else if (digitalRead(SET)) {
                    update_kp();
                }
                break;

            case CAL_PID_DERIV:
                while (!digitalRead(NAVIGATE) && !digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY);

                    Serial.print("Current kd: ");
                    Serial.println(get_kd());
                    Serial.print("Current potentiometer val: "); 
                    Serial.println(analogRead(CALIBRATION_POTENTIOMETER));
                }
                if (digitalRead(NAVIGATE)) {
                    calibration_val = CAL_EXIT;
                } else if (digitalRead(SET)) {
                    update_kd();
                }
                break;
            
            case CAL_EXIT:
                while (!digitalRead(NAVIGATE) && !digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY);
                }
                if (digitalRead(NAVIGATE)) {
                    calibration_val = CAL_TAPE_SENSOR;
                } else if (digitalRead(SET)) {
                    return;
                }
                break;
        }
        if (robot_state() != MENU) {
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
                while (!digitalRead(NAVIGATE) && !digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY);
                }
                if (digitalRead(NAVIGATE)) {
                    displayed_state = ASCEND_RAMP_MENU;
                } else if (digitalRead(SET)) {
                    switch_state(MENU, REACH_RAMP);
                }
                break;
            
            case ASCEND_RAMP_MENU:
                while (!digitalRead(NAVIGATE) && !digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY);
                }
                if (digitalRead(NAVIGATE)) {
                    displayed_state = CALIBRATE_MENU;
                } else if (digitalRead(SET)) {
                    switch_state(MENU, CALIBRATE);
                }
                break;

            case CALIBRATE_MENU:
                while (!digitalRead(NAVIGATE) && !digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY);
                }
                if (digitalRead(NAVIGATE)) {
                    displayed_state = FIND_POST_MENU;
                } else if (digitalRead(SET)) {
                    switch_state(MENU, CALIBRATE);
                }
                break;
                
            case FIND_POST_MENU:
                while (!digitalRead(NAVIGATE) && !digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY);
                }
                if (digitalRead(NAVIGATE)) {
                    displayed_state = ASCEND_RAMP_MENU;
                } else if (digitalRead(SET)) {
                    switch_state(MENU, REACH_RAMP);
                }
                break;
            
            case GET_INFINITY_STONE_MENU:
                while (!digitalRead(NAVIGATE) && !digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY);
                }
                if (digitalRead(NAVIGATE)) {
                    displayed_state = HANDLE_COLLISION_MENU;
                } else if (digitalRead(SET)) {
                    switch_state(MENU, GET_INFINITY_STONE);
                }
                break;

            case HANDLE_COLLISION_MENU:
                while (!digitalRead(NAVIGATE) && !digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY);
                }
                if (digitalRead(NAVIGATE)) {
                    displayed_state = RETURN_TO_GAUNTLET_MENU;
                } else if (digitalRead(SET)) {
                    switch_state(MENU, HANDLE_COLLISION);
                }
                break;

            case RETURN_TO_GAUNTLET_MENU:
                while (!digitalRead(NAVIGATE) && !digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY);
                }
                if (digitalRead(NAVIGATE)) {
                    displayed_state = FIT_TO_GAUNTLET_MENU;
                } else if (digitalRead(SET)) {
                    switch_state(MENU, RETURN_TO_GAUNTLET);
                }
                break;

            case FIT_TO_GAUNTLET_MENU:
                while (!digitalRead(NAVIGATE) && !digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY);
                }
                if (digitalRead(NAVIGATE)) {
                    displayed_state = GOODNIGHT_SWEET_PRINCE_MENU;
                } else if (digitalRead(SET)) {
                    switch_state(MENU, FIT_TO_GAUNTLET);
                }
                break;

            case GOODNIGHT_SWEET_PRINCE_MENU:
                while (!digitalRead(NAVIGATE) && !digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY);
                }
                if (digitalRead(NAVIGATE)) {
                    displayed_state = REACH_RAMP_MENU;
                } else if (digitalRead(SET)) {
                    switch_state(MENU, GOODNIGHT_SWEET_PRINCE);
                }
                break;
            }
        
        if (robot_state() != MENU) {
            return;
        }
    }
}