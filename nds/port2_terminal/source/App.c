#include "App.h"

#include <nds.h>
#include <stdio.h>

#include "Port2.h"


// Private
//------------------------------------------------------------------------------

void listener(u8 byte) {

	putchar(byte);
}


// Public
//------------------------------------------------------------------------------

void App_init() {

	Port2_init();
	Port2_listen(&listener);
}

void App_draw() {}

void App_frame() {

	int key = keyboardUpdate();

	if (key > 0) {
		putchar(key);
		Port2_send(key);
	}
}
