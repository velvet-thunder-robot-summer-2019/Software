#include <Arduino.h>

#define PIN_INTERRUPT PB3

void interrupt_handle();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_INTERRUPT, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT), interrupt_handle, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(1) {
    delay(1000);
  }
}

void interrupt_handle()
{
  Serial.println("Hello world");
}