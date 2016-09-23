#include <nds.h>

#include "App.h"

int main() {

	defaultExceptionHandler();

	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	consoleInit(NULL, 3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
	keyboardInit(NULL, 3, BgType_Text4bpp, BgSize_T_256x512, 20, 0, false, true);
	keyboardShow();

	App_init();

	while (1) {

		swiWaitForVBlank();

		App_draw();
		
		scanKeys();
		App_frame();
	}

	return 0;
}
