// https://circuitdigest.com/microcontroller-projects/how-to-use-interrupts-in-stm32f103c8
//#define TIM1_IRQn
//tutorial for encoder: https://www.allaboutcircuits.com/projects/how-to-use-a-rotary-encoder-in-a-mcu-based-project/?fbclid=IwAR1_uDce2J0UY5P6BcJp9nlTlienbqXadg3PnvAUFhWhTY7fyU5AdoxpciU/
// circuit diagram for encoder: http://www.farnell.com/datasheets/1678800.pdf?_ga=2.178164833.599211417.1560901559-1255588177.1559846344&fbclid=IwAR26tacgdbD9I8_XTnZPQyNHFVXE1b9wtBbOhx5ts2eWQUtFoHtJHsSzorM
// note that I swapped the 10k from leg to signal for 20kOhms

#include <Arduino.h>
#include <stm32f1xx_hal.h>

#include "AllPurposeInclude.h"
#include "GlobalInfo/Interrupts.h"

#define RETURN_TIME 130 // in seconds, arbitrary rn

void encoder_left_handle();
void encoder_right_handle();


void collision_left();
void collision_right();
void collision_front();
void collision_back();

/**
 * Does all necessary initialisation for interrupts
 */
void init_interrupts()
{
    Serial.println("init_interrupts");

    //set up pins for collision interrupt
    pinMode(BUMPER_LEFT, INPUT_PULLUP);
    pinMode(BUMPER_RIGHT, INPUT_PULLUP);

    // attach collision interrupt
    attachInterrupt(digitalPinToInterrupt(BUMPER_LEFT), set_collision_mode, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUMPER_RIGHT), set_collision_mode, FALLING);
}

/**
 * Sets state to handle collision when bumpers detect collision
 */
void set_collision_mode()
{
#if TESTING_ORDER_OF_EVENTS
Serial.println("collision at front");
#endif
    if (robot_state() != GET_INFINITY_STONE) {
        switch_state(robot_state(), HANDLE_COLLISION);
    }
}
