#import <avr/io.h>

#define SLOT2_IRQ_PIN 3

void setup() {
  Serial.begin(19200);
  pinMode(SLOT2_IRQ_PIN, OUTPUT);
}

void loop() {

  if (Serial.available() > 0) {
    char data = Serial.read();

    // Enable output to the data bus
    DDRD |= 0xf0;
    DDRB |= 0x0f;

    // Place data on the bus
    PORTD = PORTD & 0x0f | data & 0xf0;
    PORTB = PORTB & 0xf0 | data & 0x0f;

    // Send IRQ
    PORTD |= 0b00001000;
    PORTD &= 0b11110111;
    
    // Allow enough time for the data to be read
    delayMicroseconds(3);
    
    // Stop driving the bus
    DDRD &= 0x0f;
    DDRB &= 0xf0;
  }
}
