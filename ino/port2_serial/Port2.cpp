#import <avr/io.h>
#import <avr/interrupt.h>

#import "Port2.h"

volatile char Port2::data;
volatile bool Port2::unread = false;

ISR(INT0_vect) {

  Port2::isr();
}
