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

// encoder interrupts on wheels
volatile byte seqA_left = 0;
volatile byte seqB_left = 0;

volatile byte seqA_right = 0;
volatile byte seqB_right = 0;

volatile uint32_t right_wheel_dt[DT_ARRAY_SIZE] = {0};
volatile uint8_t right_wheel_dt_index = 0;

volatile uint32_t left_wheel_dt[DT_ARRAY_SIZE] = {0};
volatile uint8_t left_wheel_dt_index = 0;

/**
 * Does all necessary initialisation for interrupts
 */
void init_interrupts()
{
    Serial.println("init_interrupts");

    //set up pins for collision interrupt
    pinMode(BUMPER_LEFT, INPUT);
    pinMode(BUMPER_RIGHT, INPUT);

    // attach collision interrupt
    // attachInterrupt(digitalPinToInterrupt(BUMPER_LEFT), set_collision_mode, FALLING);
    // attachInterrupt(digitalPinToInterrupt(BUMPER_RIGHT), set_collision_mode, FALLING);

    //set up pins for encoder interrupt
    pinMode(ENCODER_LEFT_A, INPUT);
    pinMode(ENCODER_LEFT_B, INPUT);
    pinMode(ENCODER_RIGHT_A, INPUT);
    pinMode(ENCODER_RIGHT_B, INPUT);
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

// /**
//  * Increments left wheel rotations
//  */
// void encoder_left_handle()
// {
//     bool A_val = digitalRead(ENCODER_LEFT_A);
//     bool B_val = digitalRead(ENCODER_LEFT_B);
    
//     seqA_left <<= 1;
//     seqA_left |= A_val;
    
//     seqB_left <<= 1;
//     seqB_left |= B_val;

//     seqA_left &= 0b00001111;
//     seqB_left &= 0b00001111;

//     if (seqA_left == 0b00001001 && seqB_left == 0b00000011) {
//         run_status.bot_position.left_wheel_ticks++;
//         int last_index = left_wheel_dt_index == 0 ? DT_ARRAY_SIZE - 1 : left_wheel_dt_index - 1;
//         left_wheel_dt[left_wheel_dt_index++] = millis() - left_wheel_dt[last_index];
//         if (left_wheel_dt_index == DT_ARRAY_SIZE) {
//             left_wheel_dt_index = 0;
//         }
//     } else if (seqA_left == 0b00000011 && seqB_left == 0b00001001) {
//         run_status.bot_position.left_wheel_ticks--;
//     }
// }

// /**
//  * Increments right wheel rotations
//  */
// void encoder_right_handle()
// {
//     bool A_val = digitalRead(ENCODER_RIGHT_A);
//     bool B_val = digitalRead(ENCODER_RIGHT_B);
    
//     seqA_right <<= 1;
//     seqA_right |= A_val;
    
//     seqB_right <<= 1;
//     seqB_right |= B_val;

//     seqA_right &= 0b00001111;
//     seqB_right &= 0b00001111;

//     if (seqA_right == 0b00001001 && seqB_right == 0b00000011) {
//         run_status.bot_position.right_wheel_ticks++;
//         int last_index = right_wheel_dt_index == 0 ? DT_ARRAY_SIZE - 1 : right_wheel_dt_index - 1;
//         right_wheel_dt[right_wheel_dt_index++] = millis() - right_wheel_dt[last_index];
//         if (right_wheel_dt_index == DT_ARRAY_SIZE) {
//             right_wheel_dt_index = 0;
//         }
//     } else if (seqA_right == 0b00000011 && seqB_right == 0b00001001) {
//         run_status.bot_position.right_wheel_ticks--;
//     }
// }


