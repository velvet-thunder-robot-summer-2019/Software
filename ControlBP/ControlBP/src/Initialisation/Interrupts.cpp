// https://circuitdigest.com/microcontroller-projects/how-to-use-interrupts-in-stm32f103c8
#include <Arduino.h>

#include "AllPurposeInclude.h"

#define RETURN_TIME 130 // in seconds, arbitrary rn

/**
 * Does all necessary initialisation for interrupts
 */
void init_interrupts()
{
    Serial.println("init_interrupts");

    //set up pins
    pinMode(BUMPER_LEFT, INPUT);
    pinMode(BUMPER_RIGHT, INPUT);
    pinMode(BUMPER_FRONT, INPUT);
    pinMode(BUMPER_BACK, INPUT);

    // attach collision interrupt
    attachInterrupt(digitalPinToInterrupt(BUMPER_LEFT), collision_left, FALLING);

    // init timer interrupt
}

/**
 * Sets state to handle collision when bumpers detect collision
 */
void collision_left()
{
    Serial.println("collision_left");
    run_status.bot_previous_state = run_status.bot_state;
    run_status.bot_state = HANDLE_COLLISION;
    run_status.last_collision = LEFT_COLLISION;
}

/**
 * Sets state to return to gauntlet when the time reaches a threshold
 */
void timer_interrupt_handler()
{
    Serial.println("timer_interrupt_handler");
    run_status.bot_previous_state = run_status.bot_state;
    run_status.bot_state = RETURN_TO_GAUNTLET;
}