#include <stdio.h>
#include <stdlib.h>
#include "../minifb/include/MiniFB_enums.h"
#include "../minifb/include/MiniFB.h"
#include "../FreeImage/Dist/x64/FreeImage.h"
#include "WINDOW_CONSTANTS.h"

int main()
{
	struct mfb_window* window = mfb_open_ex("My display", WINDOW_WIDTH, WINDOW_HEIGHT, WF_RESIZABLE);
	uint32_t* buffer = (uint32_t*) malloc(WINDOW_WIDTH * WINDOW_HEIGHT * 4);

	level0_init(window, buffer);
	
	return 0;
}