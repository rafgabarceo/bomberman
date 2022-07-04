#include <stdio.h>
#include <stdlib.h>
#include "../minifb/include/MiniFB_enums.h"
#include "../minifb/include/MiniFB.h"
#include "../FreeImage/Dist/x64/FreeImage.h"
#include "WINDOW_CONSTANTS.h"
#include "USER.h"

int main()
{
	struct mfb_window* window = mfb_open_ex("My display", WINDOW_WIDTH, WINDOW_HEIGHT, WF_RESIZABLE);
	FIBITMAP* user_FBM = FreeImage_Load(FIF_PNG,USER_LOC, PNG_DEFAULT);
	FreeImage_FlipVertical(user_FBM);
	uint8_t* userBits = FreeImage_GetBits(user_FBM);

	// Initialize the player
	USER_DAT user = (USER_DAT){
		.imageData = userBits,
		.user_pos_x_prev = 112,
		.user_pos_y_prev = 56,
		.user_pos_x = 112,
		.user_pos_y = 56,
		.user_score = 0,
		.user_timer = 0,
		.bombsDrop = 0
	};

	mfb_set_user_data(window, (void*)&user); // Attach user data to the window

	uint32_t* buffer = (uint32_t*) malloc(WINDOW_WIDTH * WINDOW_HEIGHT * 4);
	
	while (1)
	{
		level0_init(window, buffer);
	}

	
	return 0;
}