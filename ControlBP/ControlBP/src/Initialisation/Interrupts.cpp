#include <Arduino.h>

#include "Initialisation/Interrupts.h"
#include "Initialisation/GlobalVars.h"

#define RETURN_TIME 130 // in seconds, arbitrary rn

/**
 * Does all necessary initialisation for interrupts
 */
void init_interrupts()
{
    Serial.println("init_interrupts");
    // init collision interrupt

    // init timer interrupt
}

/**
 * Sets state to handle collision when bumpers detect collision
 */
void collision_interrupt_handler()
{
    Serial.println("collision_interrupt_handler");
    bot_previous_state = bot_state;
    bot_state = HANDLE_COLLISION;
}

/**
 * Sets state to return to gauntlet when the time reaches a threshold
 */
void timer_interrupt_handler()
{
    Serial.println("timer_interrupt_handler");
    bot_previous_state = bot_state;
    bot_state = RETURN_TO_GAUNTLET;
}