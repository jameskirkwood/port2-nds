#include <nds.h>

#include "App.h"

int main() {

	defaultExceptionHandler();

	vramDefault();
	consoleDemoInit();
	lcdSwap();

	App_init();

	while (1) {

		swiWaitForVBlank();

		App_draw();
		
		scanKeys();
		App_frame();
	}

	return 0;
}
