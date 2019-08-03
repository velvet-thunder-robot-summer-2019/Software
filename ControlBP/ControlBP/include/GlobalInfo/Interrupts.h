#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#define DT_ARRAY_SIZE 3

extern volatile uint32_t right_wheel_dt[DT_ARRAY_SIZE];
extern volatile uint32_t left_wheel_dt[DT_ARRAY_SIZE];

/**
 * Does all necessary initialisation for interrupts
 */
void init_interrupts();

/**
 * Interrupt function due to time
 */
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif // INTERRUPTS_H_