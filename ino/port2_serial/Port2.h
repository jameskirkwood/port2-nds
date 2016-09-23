#import <Arduino.h>
#import <avr/io.h>
#import <avr/interrupt.h>

class Port2 {

private:

  static const int READ_BACK_PIN = 12;
  static const int BUS_FORWARD_PIN = 3;

  // The time to keep driving the NDS side of the bus after sending an IRQ
  // The IRQ is sent on the rising edge of BUS_FORWARD_PIN (active high), and
  // the NDS side of the bus is driven while the pin is asserted (high).
  // Higher values limit the outgoing data rate but improve reliability.
  static const int BUS_HOLD_TIME_MICROS = 6;

  // The time to wait between asserting the incoming data register's output
  // enable pin and reading the data from the Arduino side of the bus
  // The incoming data register drives the bus while READ_BACK_PIN (active low)
  // is asserted.
  static const int REGISTER_READ_TIME_MICROS = 1;

  // Assert or de-assert READ_BACK_PIN (active low) = pin 12
  static inline void setReadBack(bool oe) {

    if (oe) PORTB &= 0b11101111;
    else PORTB |= 0b00010000;
  }

  // Assert or de-assert BUS_FORWARD_PIN (active high) = pin 3
  static inline void setBusForward(bool oe) {

    if (oe) PORTD |= 0b00001000;
    else PORTD &= 0b11110111;
  }

  // Return the data on the Arduino side of the bus (bus mode must be INPUT)
  static inline char getBusData() {

    return PIND & 0xf0 | PINB & 0x0f;
  }

  // Output data on the Arduino side of the bus (bus mode must be OUTPUT)
  static inline void setBusData(char data) {

    PORTD = PORTD & 0x0f | data & 0xf0;
    PORTB = PORTB & 0xf0 | data & 0x0f;
  }

  // Set the data direction of the Arduino pins used for the bus
  // `mode` must be either INPUT or OUTPUT
  static inline void setBusMode(int mode) {

    if (mode == OUTPUT) {
      DDRD |= 0xf0;
      DDRB |= 0x0f;
    }
    else {
      DDRD &= 0x0f;
      DDRB &= 0xf0;
    }
  }

  // Single-byte buffer for received data
  volatile static char data;
  volatile static bool unread;

public:

  // Call in the setup function and before any other method calls
  static inline void init() {

    // Configure the register output enable pin
    pinMode(READ_BACK_PIN, OUTPUT);
    setReadBack(false);

    // Configure external interrupt 0
    pinMode(2, INPUT);     // INT0 is on pin 2
    EICRA |= (1 << ISC01); // INT0 on falling edge
    EIMSK |= (1 << INT0);  // Enable INT0

    // Configure the bus lines as inputs
    setBusMode(INPUT);

    // Configure the bus forward and slot-2 IRQ pin
    pinMode(BUS_FORWARD_PIN, OUTPUT);
    setBusForward(false);
  }

  // True if there is new data to be received
  static inline bool available() {

    return unread;
  }

  // Return the last byte received (since the buffer contains only one byte)
  static inline char receive() {

    unread = false;
    return data;
  }

  // Blindly send a byte
  static inline void send(char data) {

    // Place the data on the Arduino side of the bus
    setBusMode(OUTPUT);
    setBusData(data);

    // Forward the data to the NDS side of the bus and interrupt the NDS
    setBusForward(true);
    delayMicroseconds(BUS_HOLD_TIME_MICROS);

    // Release the both the NDS side and Arduino side of the bus
    setBusForward(false);
    setBusMode(INPUT);
  }

  // The interrupt service routine internally used by the external interrupt 0
  // handler to capture new data latched in the incoming data register
  // (not part of the Port2 interface)
  static inline void isr() {

    // Output latched data back onto the Arduino side of the bus
    setReadBack(true);
    delayMicroseconds(REGISTER_READ_TIME_MICROS);

    // Read data off the Arduino side of the bus
    data = getBusData();

    // Release the Arduino side of the bus
    setReadBack(false);

    unread = true;
  }
};
