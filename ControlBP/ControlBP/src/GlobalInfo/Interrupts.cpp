// https://circuitdigest.com/microcontroller-projects/how-to-use-interrupts-in-stm32f103c8
#define TIM1_IRQn

#include <Arduino.h>
#include <stm32f1xx_hal.h>

#include "AllPurposeInclude.h"
#include "GlobalInfo/Interrupts.h"

#define RETURN_TIME 130 // in seconds, arbitrary rn


#ifdef __cplusplus
extern "C" {
#endif

#define BLINKY PC13

// void initialise_timer();
// void SysTick_Handler(void);
 
static TIM_HandleTypeDef s_TimerInstance = { 
    .Instance = TIM1
};

extern "C" void TIM1_IRQHandler()
{
    //this segment copied and pasted from HAL_TIM_IRQHandler()
  /* TIM Update event */
  if(__HAL_TIM_GET_FLAG(&s_TimerInstance, TIM_FLAG_UPDATE) != RESET)
  {
    if(__HAL_TIM_GET_IT_SOURCE(&s_TimerInstance, TIM_IT_UPDATE) !=RESET)
    {
      __HAL_TIM_CLEAR_IT(&s_TimerInstance, TIM_IT_UPDATE);
      timer_interrupt_handler();
      // HAL_TIM_PeriodElapsedCallback(&s_TimerInstance);
    }
  }
}

/* 
extern "C" void TIM1_IRQHandler()
{
    HAL_TIM_IRQHandler(&s_TimerInstance);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    digitalWrite(BLINKY, !digitalRead(BLINKY));
}*/

/**
 * Taken from tutorial: https://visualgdb.com/tutorials/arm/stm32/timers/
 */

void SysTick_Handler(void)
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

#ifdef __cplusplus
}
#endif


void initialise_timer()
{
    // setup timer
    __HAL_RCC_TIM1_CLK_ENABLE();
    s_TimerInstance.Init.Prescaler = 40000;
    s_TimerInstance.Init.CounterMode = TIM_COUNTERMODE_UP;
    s_TimerInstance.Init.Period = 500;
    s_TimerInstance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    s_TimerInstance.Init.RepetitionCounter = 0;
    HAL_TIM_Base_Init(&s_TimerInstance);
    // HAL_TIM_Base_Start(&s_TimerInstance); /// maybe
    HAL_TIM_Base_Start_IT(&s_TimerInstance);

    // setup IRQ
    HAL_NVIC_SetPriority(TIM1_IRQn, 0, 0); //what priority am I even setting? check
    HAL_NVIC_EnableIRQ(TIM1_IRQn);
}

/**
 * Does all necessary initialisation for interrupts
 */
void init_interrupts()
{
    Serial.println("init_interrupts");
    pinMode(BLINKY, OUTPUT);

    //set up pins
    // pinMode(BUMPER_LEFT, INPUT);
    // pinMode(BUMPER_RIGHT, INPUT);
    // pinMode(BUMPER_FRONT, INPUT);
    // pinMode(BUMPER_BACK, INPUT);

    // attach collision interrupt
    // attachInterrupt(digitalPinToInterrupt(BUMPER_LEFT), collision_left, RISING);
    // attachInterrupt(digitalPinToInterrupt(BUMPER_RIGHT), collision_right, RISING);
    // attachInterrupt(digitalPinToInterrupt(BUMPER_FRONT), collision_front, RISING);
    // attachInterrupt(digitalPinToInterrupt(BUMPzER_BACK), collision_back, RISING);

    // init timer interrupt
    initialise_timer();
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
    digitalWrite(BLINKY, !digitalRead(BLINKY));
    Serial.println("timer_interrupt_handler");
    // switch_state(robot_state(), RETURN_TO_GAUNTLET);
}
