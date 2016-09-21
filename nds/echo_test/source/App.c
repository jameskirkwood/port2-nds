#include "App.h"

#include <nds.h>
#include <stdio.h>

#include "Port2.h"

#define TICKS_TO_MICROS(ticks) ((ticks) / 33.513982)


// Private
//------------------------------------------------------------------------------

static int echo_ticks;
static float echo_micros;
static float average_micros;
static int timeouts;


// Public
//------------------------------------------------------------------------------

void App_init() {

	Port2_init();

	average_micros = -1.0; // no data
	timeouts = 0;
}

void App_draw() {

	consoleClear();

	if (average_micros < 0) {
		printf("no data");
	}
	else if (echo_ticks == -1) {
		printf("echo timeout");
	}
	else {
		printf(
			"   ticks | microseconds\n"
			"%8d | %12.2f\n\n"
			"running avg: %10.2f\n\n"
			"timeouts:    %10d\n",
			echo_ticks, echo_micros,
			average_micros, timeouts
		);
	}
}

void App_frame() {

	echo_ticks = Port2_echo();

	if (echo_ticks == -1) {
		timeouts++;
	}
	else {
		echo_micros = TICKS_TO_MICROS(echo_ticks);

		if (average_micros < 0) {
			// no data
			average_micros = echo_micros;
		}
		else {
			average_micros = (average_micros * 249 + echo_micros) * 0.004;
		}
	}
}
