#include <stdio.h>
#include <stdlib.h>
#include "../minifb/include/MiniFB_enums.h"
#include "../minifb/include/MiniFB.h"
#include "../FreeImage/Dist/x64/FreeImage.h"

int main()
{
	struct mfb_window* window = mfb_open_ex("My display", 800, 600, WF_RESIZABLE);
	uint32_t* buffer = (uint32_t*) malloc(800*600*400);

	for (int i = 0; i < 200; i++)
	{
		for (int j = 0; j < 800; j++)
		{
			buffer[800 * i + j] = 0xFF0000;
		}
	}

	while (mfb_wait_sync(window))
	{
		int state = mfb_update_ex(window, buffer, 800, 600);
		if (state < 0)
		{
			window = NULL;
			break;
		}
	}
	
	return 0;
}