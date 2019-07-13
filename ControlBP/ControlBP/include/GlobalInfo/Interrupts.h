#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

/**
 * Does all necessary initialisation for interrupts
 */
void init_interrupts();

/**
 * Sets state to handle collision when bumpers detect collision from left
 */
void collision_left();

/**
 * Sets state to handle collision when bumpers detect collision from right
 */
void collision_right();

/**
 * Sets state to handle collision when bumpers detect collision from front
 */
void collision_front();

/**
 * Sets state to handle collision when bumpers detect collision from back
 */
void collision_back();

/**
 * Sets state to return to gauntlet when the time reaches a threshold
 */
void timer_interrupt_handler();

/**
 * Interrupt function due to time
 */
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif // INTERRUPTS_H_