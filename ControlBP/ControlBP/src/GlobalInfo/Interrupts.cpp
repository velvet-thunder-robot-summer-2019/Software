// https://circuitdigest.com/microcontroller-projects/how-to-use-interrupts-in-stm32f103c8
#include <Arduino.h>

#include "AllPurposeInclude.h"
#include "GlobalInfo/Interrupts.h"

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
    // attachInterrupt(digitalPinToInterrupt(BUMPER_LEFT), collision_left, FALLING);
    // attachInterrupt(digitalPinToInterrupt(BUMPER_RIGHT), collision_right, FALLING);
    // attachInterrupt(digitalPinToInterrupt(BUMPER_FRONT), collision_front, FALLING);
    // attachInterrupt(digitalPinToInterrupt(BUMPER_BACK), collision_back, FALLING);

    // init timer interrupt
}

/**
 * Sets state to handle collision when bumpers detect collision
 */
void collision_left()
{
    Serial.println("collision_left");
    switch_state(robot_state(), HANDLE_COLLISION);
    run_status.last_collision = LEFT_COLLISION;
}

/**
 * Sets state to handle collision when bumpers detect collision from right
 */
void collision_right()
{
    Serial.println("collision_right");
    switch_state(robot_state(), HANDLE_COLLISION);
    run_status.last_collision = RIGHT_COLLISION;
}

/**
 * Sets state to handle collision when bumpers detect collision from front
 */
void collision_front()
{
    Serial.println("collision_front");
    switch_state(robot_state(), HANDLE_COLLISION);
    run_status.last_collision = FRONT_COLLISION;
}

/**
 * Sets state to handle collision when bumpers detect collision from back
 */
void collision_back()
{
    Serial.println("collision_back");
    switch_state(robot_state(), HANDLE_COLLISION);
    run_status.last_collision = BACK_COLLISION;
}

/**
 * Sets state to return to gauntlet when the time reaches a threshold
 */
void timer_interrupt_handler()
{
    Serial.println("timer_interrupt_handler");
    switch_state(robot_state(), RETURN_TO_GAUNTLET);
}