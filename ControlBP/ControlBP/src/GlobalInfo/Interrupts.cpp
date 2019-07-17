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
#define BLINKY PC13

// encoder interrupts on wheels
volatile byte seqA_left = 0;
volatile byte seqB_left = 0;

volatile byte seqA_right = 0;
volatile byte seqB_right = 0;


/**
 * Does all necessary initialisation for interrupts
 */
void init_interrupts()
{
    Serial.println("init_interrupts");
    pinMode(BLINKY, OUTPUT);

    //set up pins for collision interrupt
    pinMode(BUMPER_LEFT, INPUT);
    pinMode(BUMPER_RIGHT, INPUT);
    pinMode(BUMPER_FRONT, INPUT);
    pinMode(BUMPER_BACK, INPUT);

    // attach collision interrupt
    attachInterrupt(digitalPinToInterrupt(BUMPER_LEFT), collision_left, RISING);
    attachInterrupt(digitalPinToInterrupt(BUMPER_RIGHT), collision_right, RISING);
    attachInterrupt(digitalPinToInterrupt(BUMPER_FRONT), collision_front, RISING);
    attachInterrupt(digitalPinToInterrupt(BUMPER_BACK), collision_back, RISING);

    // init timer interrupt
    // initialise_timer();

    //set up pins for encoder interrupt
    pinMode(ENCODER_LEFT_A, INPUT);
    pinMode(ENCODER_LEFT_B, INPUT);
    pinMode(ENCODER_RIGHT_A, INPUT);
    pinMode(ENCODER_RIGHT_B, INPUT);
    
    attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_A), encoder_left_handle, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_B), encoder_left_handle, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_A), encoder_right_handle, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_B), encoder_right_handle, CHANGE);
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


/**
 * Increments left wheel rotations
 */
void encoder_left_handle()
{
    bool A_val = digitalRead(ENCODER_LEFT_A);
    bool B_val = digitalRead(ENCODER_LEFT_B);
    
    seqA_left <<= 1;
    seqA_left |= A_val;
    
    seqB_left <<= 1;
    seqB_left |= B_val;

    seqA_left &= 0b00001111;
    seqB_left &= 0b00001111;

    if (seqA_left == 0b00001001 && seqB_left == 0b00000011) {
        run_status.bot_position.left_wheel_rotations++;
    } else if (seqA_left == 0b00000011 && seqB_left == 0b00001001) {
        run_status.bot_position.left_wheel_rotations--;
    }
}

/**
 * Increments right wheel rotations
 */
void encoder_right_handle()
{
    bool A_val = digitalRead(ENCODER_RIGHT_A);
    bool B_val = digitalRead(ENCODER_RIGHT_B);
    
    seqA_right <<= 1;
    seqA_right |= A_val;
    
    seqB_right <<= 1;
    seqB_right |= B_val;

    seqA_right &= 0b00001111;
    seqB_right &= 0b00001111;

    if (seqA_right == 0b00001001 && seqB_right == 0b00000011) {
        run_status.bot_position.right_wheel_rotations++;
    } else if (seqA_right == 0b00000011 && seqB_right == 0b00001001) {
        run_status.bot_position.right_wheel_rotations--;
    }
}




/*
#ifdef __cplusplus
extern "C" {
#endif

// void initialise_timer();
// void SysTick_Handler(void);
 
static TIM_HandleTypeDef s_TimerInstance = { 
    .Instance = TIM1
};

extern "C" void TIM1_IRQHandler()
{*/
    //this segment copied and pasted from HAL_TIM_IRQHandler()
  /* TIM Update event */
/*   if(__HAL_TIM_GET_FLAG(&s_TimerInstance, TIM_FLAG_UPDATE) != RESET)
  {
    if(__HAL_TIM_GET_IT_SOURCE(&s_TimerInstance, TIM_IT_UPDATE) !=RESET)
    {
      __HAL_TIM_CLEAR_IT(&s_TimerInstance, TIM_IT_UPDATE);
      timer_interrupt_handler();
      // HAL_TIM_PeriodElapsedCallback(&s_TimerInstance);
    }
  }
}*/

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
/*
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
}*/