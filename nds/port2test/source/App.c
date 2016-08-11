#include "App.h"

#include <nds.h>
#include <stdio.h>

#include "Port2.h"


// Private
//------------------------------------------------------------------------------

typedef enum {
	APP_IDLE,
	APP_PING,
	APP_LISTEN
} App_status_t;

static App_status_t App_status;

static int ping_ticks;
static float ping_usecs;
static float average_usecs;
static int timeouts;

void ping() {

	ping_ticks = Port2_ping();
	ping_usecs = ping_ticks / 33.513982;
}

void listener(u8 byte) {

	printf("%c", byte);
}


// Public
//------------------------------------------------------------------------------

void App_init() {

	Port2_init();
	App_status = APP_IDLE;
}

void App_draw() {

	switch (App_status) {

		case APP_IDLE:
		consoleClear();
		printf(
			"A: ping device at 60Hz\n"
			"B: listen for input from device\n\n"
		);
		break;

		case APP_PING:
		consoleClear();
		if (ping_ticks == -1) {
			printf("ping timeout");
		}
		else {
			printf(
				"   ticks | microseconds\n"
				"%8d | %12.2f\n\n"
				"running avg: %10.2f\n\n"
				"timeouts:    %10d\n",
				ping_ticks, ping_usecs,
				average_usecs, timeouts
			);
		}
		break;

		case APP_LISTEN:
		break;
	}
}

void App_frame() {

	switch (App_status) {

		case APP_IDLE:
		if (keysDown() & KEY_A) {
			ping();
			average_usecs = ping_usecs;
			timeouts = 0;
			App_status = APP_PING;
		}
		else if (keysDown() & KEY_B) {
			consoleClear();
			printf("Listening...\n");
			Port2_listen(&listener);
			App_status = APP_LISTEN;
		}
		break;

		case APP_PING:
		ping_ticks = Port2_ping();
		if (ping_ticks == -1) {
			timeouts++;
		}
		else {
			ping_usecs = ping_ticks / 33.513982;
			average_usecs = (average_usecs * 249 + ping_usecs) * 0.004;
		}
		if (keysDown() & KEY_A) {
			App_status = APP_IDLE;
		}
		break;

		case APP_LISTEN:
		if (keysDown() & KEY_B) {
			Port2_listen(0);
			App_status = APP_IDLE;
		}
		break;
	}
}
