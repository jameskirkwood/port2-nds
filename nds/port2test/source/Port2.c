#include "Port2.h"

#include <nds.h>


// Private
//------------------------------------------------------------------------------

static volatile enum {
	PORT2_IDLE,
	PORT2_IRQ_WAIT
} Port2_status;

// slot 2 IRQ handler when waiting for a ping reply
// (stops timer 0 and changes to idle state)
void ping_isr() {

	TIMER0_CR = 0;
	Port2_status = PORT2_IDLE;
}

static void (*input_listener)(u8);

// slot 2 IRQ handler when listening for bytes
void listen_isr() {

	(*input_listener)(SRAM[0]);
}


// Public
//------------------------------------------------------------------------------

void Port2_init() {

	sysSetCartOwner(BUS_OWNER_ARM9);
	irqDisable(IRQ_CART);
	Port2_status = PORT2_IDLE;
}

int Port2_ping() {

	Port2_status = PORT2_IRQ_WAIT;

	irqSet(IRQ_CART, &ping_isr);
	irqEnable(IRQ_CART);

	TIMER0_CR = 0;
	TIMER0_DATA = 0;
	TIMER0_CR = TIMER_ENABLE;

	SRAM[0] = 0;

	while (Port2_status == PORT2_IRQ_WAIT) {

		// timeout after 1 millisecond
		if (TIMER0_DATA > 33514) {
			TIMER0_CR = 0;
			irqDisable(IRQ_CART);
			return -1;
		}
	}

	irqDisable(IRQ_CART);
	return TIMER0_DATA;
}

void Port2_listen(void (*listener)(u8)) {

	if (listener) {

		input_listener = listener;

		irqSet(IRQ_CART, &listen_isr);
		irqEnable(IRQ_CART);
	}
	else {
		irqDisable(IRQ_CART);
	}
}
