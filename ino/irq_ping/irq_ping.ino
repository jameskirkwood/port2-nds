#import <avr/io.h>
#import <avr/interrupt.h>

#define INT0_PIN 2
#define SLOT2_IRQ_PIN 3

ISR(INT0_vect) {
  PORTD |= 0b00001000; // Turn on pin 3
  PORTD &= 0b11110111; // Turn off pin 3
}

void setup() {
  pinMode(SLOT2_IRQ_PIN, OUTPUT);
  pinMode(INT0_PIN, INPUT);
  EICRA |= (2 << ISC00); // INT0 on falling edge
  EIMSK |= (1 << INT0);  // Enable INT0
}

void loop() {}
