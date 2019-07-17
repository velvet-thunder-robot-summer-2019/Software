//tutorial for encoder: https://www.allaboutcircuits.com/projects/how-to-use-a-rotary-encoder-in-a-mcu-based-project/?fbclid=IwAR1_uDce2J0UY5P6BcJp9nlTlienbqXadg3PnvAUFhWhTY7fyU5AdoxpciU/
#include <Arduino.h>

#define PIN_INTERRUPT PB3

#define ENCODER_INTERRUPT_A PB5 // left side pin
#define ENCODER_INTERRUPT_B PB4 // right side of pin

// #define DEBOUNCE_TIME 5 // ms

void interrupt_handle();
void encoder_interrupt_handle();
// void encoder_interrupt_handle_rising();
// void encoder_interrupt_handle_falling();

// volatile int rotation_count = 0;
volatile byte seqA = 0;
volatile byte seqB = 0;
volatile byte cnt1 = 0;
volatile byte cnt2 = 0;

// volatile int plus = 0;
// volatile int minus = 0;

// uint32_t last_encoder_interrupt_rising = 0;
// uint32_t last_encoder_interrupt_falling = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // collision interrupt
  // pinMode(PIN_INTERRUPT, INPUT);
  // attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT), interrupt_handle, FALLING);

  // encoder interrupt
  pinMode(ENCODER_INTERRUPT_A, INPUT);
  pinMode(ENCODER_INTERRUPT_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_INTERRUPT_A), encoder_interrupt_handle, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_INTERRUPT_B), encoder_interrupt_handle, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(1) {
    delay(500);
    Serial.print("count 1: ");
    Serial.println(cnt1);
    Serial.print("count 2: ");
    Serial.println(cnt2);
    // Serial.print("Count: ");
    // Serial.println(rotation_count);
    // Serial.print("plus direction: ");
    // Serial.println(plus);
    // Serial.print("minus direction: ");
    // Serial.println(minus);
  }
}

void interrupt_handle()
{
  Serial.println("Hello world");
}

// void encoder_interrupt_handle_falling()
// {

//   bool direction = !digitalRead(ENCODER_CHECK);

//   if (direction) {
//     rotation_count++;
//     plus++;
//   } else {
//     rotation_count--;
//     minus++;
//   }
// }

// void encoder_interrupt_handle_rising()
// {
//   bool direction = digitalRead(ENCODER_CHECK);

//   if (direction) {
//     rotation_count++;
//     plus++;
//   } else {
//     rotation_count--;
//     minus++;
//   }
// }

void encoder_interrupt_handle()
{
  bool A_val = digitalRead(ENCODER_INTERRUPT_A);
  bool B_val = digitalRead(ENCODER_INTERRUPT_B);

  seqA <<= 1;
  seqA |= A_val;

  seqB <<= 1;
  seqB |= B_val;

  seqA &= 0b00001111;
  seqB &= 0b00001111;

  if (seqA == 0b00001001 && seqB == 0b00000011) {
    cnt1++;
  } else if (seqA == 0b00000011 && seqB == 0b00001001) {
    cnt2++;
  }
}