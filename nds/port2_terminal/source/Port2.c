#include "Port2.h"

#include <nds.h>


// Private
//------------------------------------------------------------------------------

static void (*input_listener)(u8);

// slot 2 IRQ handler when listening
void listen_isr() {

	(*input_listener)(SRAM[0]);
}


// Public
//------------------------------------------------------------------------------

void Port2_init() {

	sysSetCartOwner(BUS_OWNER_ARM9);
	irqDisable(IRQ_CART);
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

void Port2_send(u8 data) {

	SRAM[0] = data;
}
