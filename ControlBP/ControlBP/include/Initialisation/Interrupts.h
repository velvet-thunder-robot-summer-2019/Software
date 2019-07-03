#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

/**
 * Sets state to handle collision when bumpers detect collision
 */
void collision_interrupt_handler();

/**
 * Sets state to return to gauntlet when the time reaches a threshold
 */
void timer_interrupt_handler();

#endif // INTERRUPTS_H_