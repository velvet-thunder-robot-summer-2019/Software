#include <Arduino.h>

#include "Debugging/Menu.h"
#include "Debugging/Debug.h"
#include "Locomotion/PID.h"
#include "AllPurposeInclude.h"

#define MENU_REFRESH_DELAY 400 //ms, bring this down once display set up


// OLED stuff
#include <Wire.h>
#include <Debugging/OLED/Adafruit_SSD1306.h>
#include <Debugging/OLED/FreeMono12pt7b.h>


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//------------

#define OLED_RESET -1  // Not used
Adafruit_SSD1306 display(OLED_RESET);

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
void display_kd();
void display_kp();
void display_tape_sensor_threshold();
void display_exit();

void init_menu()
{
    pinMode(SET, INPUT);
    pinMode(NAVIGATE, INPUT);
    pinMode(MASTER_SWITCH, INPUT);
    pinMode(THANOS_v_METHANOS_SWITCH, INPUT);

    // init OLED
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    // display.setCursor(0,0);
    // display.println("OLED Display 128x64");
    // display.setFont(&FreeMono12pt7b);
    // display.drawPixel(0,45,WHITE);
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
                display.clearDisplay();
                display.setCursor(30, 30);
                display.println("CAL");
                display.display();
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
                display.clearDisplay();
                display.setCursor(30, 30);
                display.println("DEBUG");
                display.display();
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
                display.clearDisplay();
                display.setCursor(30, 30);
                display.println("STATE");
                display.display();
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

                display_tape_sensor_threshold();
                
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
                        display_tape_sensor_threshold();
                    }

                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    calibration_val = CAL_PID_PROP;
                } else if (!digitalRead(SET)) {
                    update_threshold_tape_sensor();
                    display_tape_sensor_threshold();
                }
                break;

            case CAL_PID_PROP:
                Serial.print("Current kp: ");
                Serial.println(get_kp());
                Serial.print("Current potentiometer val: "); 
                Serial.println(analogRead(CALIBRATION_POTENTIOMETER));
                display_kp();
                
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
                        display_kp();
                    }

                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    calibration_val = CAL_PID_DERIV;
                } else if (!digitalRead(SET)) {
                    Serial.print("kd updated");
                    update_kp();
                    display_kp();
                }
                break;

            case CAL_PID_DERIV:
                Serial.print("Current kd: ");
                Serial.println(get_kd());
                Serial.print("Current potentiometer val: "); 
                Serial.println(analogRead(CALIBRATION_POTENTIOMETER));
                display_kd();

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
                        display_kd();
                    }                

                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    calibration_val = CAL_EXIT;
                } else if (!digitalRead(SET)) {
                    Serial.print("kd updated");
                    update_kd();
                    display_kd();
                }
                break;
            
            case CAL_EXIT:
                Serial.println("Exit");
                display_exit();

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
                display.clearDisplay();
                display.setCursor(5, 30);
                display.println("Reach ramp");
                display.display();
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
                display.clearDisplay();
                display.setCursor(5, 30);
                display.println("Ascend ramp");
                display.display();
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
                display.clearDisplay();
                display.setCursor(5, 30);
                display.println("find post");
                display.display();
                while (digitalRead(NAVIGATE) && digitalRead(SET)) {
                    delay(MENU_REFRESH_DELAY / 10);
                }
                if (!digitalRead(NAVIGATE)) {
                    displayed_state = GET_INFINITY_STONE_MENU;
                } else if (!digitalRead(SET)) {
                    switch_state(MENU, REACH_RAMP);
                    Serial.println("Entering find post state");
                    if (run_status.bot_identity == THANOS) {
                        update_position(THANOS_INTERSECTION, POST_4);
                    } else {
                        update_position(METHANOS_INTERSECTION, POST_1);
                    }
                }
                break;
            
            case GET_INFINITY_STONE_MENU:
                Serial.println("get infinity stone menu");
                display.clearDisplay();
                display.setCursor(5, 30);
                display.println("get inf");
                display.display();
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
                display.clearDisplay();
                display.setCursor(5, 30);
                display.println("handle coll");
                display.display();
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
                display.clearDisplay();
                display.setCursor(5, 30);
                display.println("return");
                display.display();
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
                display.clearDisplay();
                display.setCursor(5, 30);
                display.println("fit to gaunt");
                display.display();
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
                display.clearDisplay();
                display.setCursor(5, 30);
                display.println("'night'");
                display.display();
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
                display_exit(); 
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

void display_tape_sensor_threshold()
{                        
    display.clearDisplay();
    display.setCursor(0, 15);
    display.print("Tape: ");
    display.println(get_tape_sensor_threshold());
    display.print("Pot: ");
    display.println(analogRead(CALIBRATION_POTENTIOMETER));
    display.display();
}

void display_kp()
{                        
    display.clearDisplay();
    display.setCursor(0, 15);
    display.print("kp: ");
    display.println(get_kp());
    display.print("Pot: ");
    display.println(analogRead(CALIBRATION_POTENTIOMETER));
    display.display();
}

void display_kd()
{                        
    display.clearDisplay();
    display.setCursor(0, 15);
    display.print("kd: ");
    display.println(get_kd());
    display.print("Pot: ");
    display.println(analogRead(CALIBRATION_POTENTIOMETER));
    display.display();
}

void display_exit()
{
    display.clearDisplay();
    display.setCursor(30, 30);
    display.println("EXIT");
    display.display();
}

void stub_arm_motion() 
{
    display.clearDisplay();
    display.setCursor(30, 30);
    display.println("Arm");
    display.display();
    delay(5000);
    display.clearDisplay();
}