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

static menu_page menu_display;

void calibration_menu();
void debug_menu();
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
                debug_menu();
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
                    delay(1);
                }
                if (digitalRead(NAVIGATE)) {
                    calibration_val = CAL_TAPE_SENSOR;
                } else if (digitalRead(SET)) {
                    return;
                }
                break;
        }
    }
}