#include "WINDOW_CONSTANTS.h"
#include "../minifb/include/Minifb.h"
#include "../FreeImage/Dist/x64/FreeImage.h"
#include "../minifb/include/MiniFB_enums.h"

void mainMenu_init(struct mfb_window* window, uint32_t* framebuffer)
{
	mfb_set_keyboard_callback(window, mainMenu_keyboard_callback);

	// Loads the background image
	FIBITMAP* background_Menu = FreeImage_Load(FIF_PNG, BGMAINMENU, PNG_DEFAULT);
	FreeImage_FlipVertical(background_FBM);
	uint8_t* backgroundBits = FreeImage_GetBits(background_FBM);

	while (1)
	{

		int state = mfb_update_ex(window, (void*)framebuffer, WINDOW_WIDTH, WINDOW_HEIGHT);

		// Load the background into the page. 
		for (int i = 0; i < (BACKGROUND_HEIGHT * BACKGROUND_WIDTH * 4); i += 4)
		{
			framebuffer[i / 4] = (backgroundBits[i + 2] << 16) | (backgroundBits[i + 1] << 8) | (backgroundBits[i]);
		}


		if (state < 0)
		{
			// Clean functions to prevent any memory leaks
			FreeImage_Unload(corner_FBM);
			FreeImage_Unload(wall_FBM);
			FreeImage_Unload(background_FBM);
			window = NULL;
			break;
		}
	}

	// Clean functions to prevent any memory leaks
	FreeImage_Unload(corner_FBM);
	FreeImage_Unload(wall_FBM);
	FreeImage_Unload(background_FBM);
}

};


void MBtnCall(struct mfb_window* window, mfb_mouse_button button, mfb_key_mod mod, bool isPressed) {
	state_data* Sdata = (state_data*)mfb_get_user_data(window);

	static int BtnState = 0;
	
	int MX = mfb_get_mouse_x(window);
	int MY = mfb_get_mouse_y(window);

	//Menu callbacks
	if (BtnState == 0) {
		//play
		if (MX >= 565 && MX <= 715) && (MY >= 292 && MY <= 366) {
			//printf("check1\n");

			//printf("check2\n");
			if (button == 1) {
				*(Sdata->state) = 1;
				BtnState = 1;
				
				level0_init(window, buffer);
				// printf("A key was pressed");
			}
		}
		//quit
		else if (MX >= 559 && MX <= 717) && (MY >= 435 && MY <= 504) {
			//printf("check2\n");
			if (button == 1) {
				mfb_close(window);
				mfb_close(window);
				// printf("A key was pressed\n");

			}
		}
	}

}