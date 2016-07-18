#include "App.h"

#include <nds.h>
#include <stdio.h>

#include "Port2.h"


// Private
//------------------------------------------------------------------------------

typedef enum {
	APP_IDLE,
	APP_INIT,
	APP_PING_ONCE_RESULTS,
	APP_PING_CONTINUOUS
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


// Public
//------------------------------------------------------------------------------

void App_init() {

	Port2_init();
	App_status = APP_INIT;
}

void App_draw() {

	if (App_status == APP_IDLE) return;

	consoleClear();
	printf(
		"NDS <-> Arduino interrupt test\n"
		"A: ping Arduino\n"
		"B: toggle 60Hz pinging\n\n"
	);

	switch (App_status) {

		case APP_PING_ONCE_RESULTS:
		case APP_PING_CONTINUOUS:
		if (ping_ticks == -1) {
			printf("ping timeout");
		}
		else {
			printf(
				"   ticks | microseconds\n"
				"%8d | %12.2f\n\n",
				ping_ticks, ping_usecs
			);
			if (App_status == APP_PING_CONTINUOUS) {
				printf(
					"running avg: %10.2f\n\n"
					"timeouts:    %10d\n",
					average_usecs, timeouts
				);
			}
		}
		break;

		default:
		break;
	}
}

void App_frame() {

	switch (App_status) {

		case APP_IDLE:
		if (keysDown() & KEY_A) {
			ping();
			App_status = APP_PING_ONCE_RESULTS;
		}
		else if (keysDown() & KEY_B) {
			ping();
			average_usecs = ping_usecs;
			timeouts = 0;
			App_status = APP_PING_CONTINUOUS;
		}
		break;

		case APP_INIT:
		case APP_PING_ONCE_RESULTS:
		App_status = APP_IDLE;
		break;

		case APP_PING_CONTINUOUS:
		ping_ticks = Port2_ping();
		if (ping_ticks == -1) {
			timeouts++;
		}
		else {
			ping_usecs = ping_ticks / 33.513982;
			average_usecs = (average_usecs * 249 + ping_usecs) * 0.004;
		}
		if (keysDown() & KEY_B) {
			App_status = APP_IDLE;
		}
		break;
	}
}
