#include "LEVEL0_DEFINES.h"
#include "WINDOW_CONSTANTS.h"
#include "../minifb/include/Minifb.h"
#include "../FreeImage/Dist/x64/FreeImage.h"
#include "../minifb/include/MiniFB_enums.h"

void level0_init(struct mfb_window* window, uint32_t* framebuffer)
{
	// Loads the corner image
	FIBITMAP* corner_FBM = FreeImage_Load(FIF_PNG, CORNER_BLOCK_LOC, PNG_DEFAULT);
	FreeImage_FlipVertical(corner_FBM);
	uint8_t* cornerBits = FreeImage_GetBits(corner_FBM);

	// Loads the wall images
	FIBITMAP* wall_FBM = FreeImage_Load(FIF_PNG, WALL_LOC, PNG_DEFAULT);
	FreeImage_FlipVertical(wall_FBM);
	uint8_t* wallBits = FreeImage_GetBits(wall_FBM);

	uint8_t* wallBitsHorizontal = FreeImage_GetBits(FreeImage_Rotate(wall_FBM, 90, 128));

	// Loads the background image
	FIBITMAP* background_FBM = FreeImage_Load(FIF_PNG, BACKGROUND_LOC, PNG_DEFAULT);
	FreeImage_FlipVertical(background_FBM);
	uint8_t* backgroundBits = FreeImage_GetBits(background_FBM);

	// Load the background into the page. MIGHT MOVE TO WHILE LOOP AS USERS AND ENEMIES WILL BE MOVING
	for (int i = 0; i < (BACKGROUND_HEIGHT * BACKGROUND_WIDTH * 4); i += 4)
	{
		framebuffer[i / 4] = (backgroundBits[i + 2] << 16) | (backgroundBits[i + 1] << 8) | (backgroundBits[i]);
	}

	static int cornersX[4] = { 0, // X corners coordinates
							BACKGROUND_WIDTH - 56,
							0,
							BACKGROUND_WIDTH - 56
	};
	static int cornersY[4] = { 0, // Y corners coordinates
								0,
								BACKGROUND_HEIGHT - 56,
								BACKGROUND_HEIGHT - 56
	};

	// Generate the metadata for the corner blocks
	Block corners[4];
	for (int i = 0; i < 4; i++)
	{
		corners[i] = (Block){
			.imageData = cornerBits,
			.state = PRESENT,
			.type = WALL_BLOCK,
			.x_position = cornersX[i],
			.y_position = cornersY[i]
		};
	}

	// Generate wall data
	Block wallsTop[25];
	Block wallsRight[11];
	Block wallsBottom[25];
	Block wallsLeft[11];

	for (int i = 0; i < 25; i++)
	{
		wallsTop[i] = (Block){
			.imageData = wallBitsHorizontal,
			.state = PRESENT,
			.type = WALL_BLOCK,
			.x_position = 56 * i,
			.y_position = 0
		};

		wallsBottom[i] = (Block){
			.imageData = wallBitsHorizontal,
			.state = PRESENT,
			.type = WALL_BLOCK,
			.x_position = 56 * i,
			.y_position = BACKGROUND_HEIGHT - 56
		};
	}

	for (int i = 0; i < 11; i++)
	{
		wallsRight[i] = (Block){
			.imageData = wallBits,
			.state = PRESENT,
			.type = WALL_BLOCK,
			.x_position = 0,
			.y_position = 56*i
		};

		wallsLeft[i] = (Block){
			.imageData = wallBits,
			.state = PRESENT,
			.type = WALL_BLOCK,
			.x_position = BACKGROUND_WIDTH - 56,
			.y_position = 56*i
		};
	}



	while (1)
	{

		int state = mfb_update_ex(window, (void*)framebuffer, WINDOW_WIDTH, WINDOW_HEIGHT);
		generate_walls_and_corners(window, framebuffer, wallsTop, wallsBottom, wallsRight, wallsLeft, corners);

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

void generate_walls_and_corners(struct mfb_window* window, uint32_t* framebuffer, Block* wallsTop, Block* wallsBottom, Block* wallsRight, Block* wallsLeft, Block* corners)
{
	// Draw the top and bottom walls
	for (int k = 0; k < 25; k++)
	{
		for (int i = 0; i < BLOCK_HEIGHT; i++)
		{
			for (int j = 0; j < BLOCK_WIDTH; j++)
			{
				uint8_t r = wallsTop[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 2];
				uint8_t g = wallsTop[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 1];
				uint8_t b = wallsTop[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j];
				uint32_t pixel = (r << 16) | (g << 8) | b;
				if (pixel) framebuffer[BACKGROUND_WIDTH * (i + wallsTop[k].y_position) + (j + wallsTop[k].x_position)] = pixel; // access the location in the framebuffer that we want to overwrite. 

				r = wallsBottom[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 2];
				g = wallsBottom[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 1];
				b = wallsBottom[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j];
				if (pixel) framebuffer[BACKGROUND_WIDTH * (i + wallsBottom[k].y_position) + (j + wallsBottom[k].x_position)] = pixel;

			}
		}
	}

	// Draw the right and left walls
	for (int k = 0; k < 11; k++)
	{
		for (int i = 0; i < BLOCK_HEIGHT; i++)
		{
			for (int j = 0; j < BLOCK_WIDTH; j++)
			{
				uint8_t r = wallsLeft[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 2];
				uint8_t g = wallsLeft[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 1];
				uint8_t b = wallsLeft[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j];
				uint32_t pixel = (r << 16) | (g << 8) | b;
				if (pixel) framebuffer[BACKGROUND_WIDTH * (i + wallsLeft[k].y_position) + (j + wallsLeft[k].x_position)] = pixel; // access the location in the framebuffer that we want to overwrite. 

				r = wallsRight[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 2];
				g = wallsRight[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 1];
				b = wallsRight[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j];
				if (pixel) framebuffer[BACKGROUND_WIDTH * (i + wallsRight[k].y_position) + (j + wallsRight[k].x_position)] = pixel;

			}
		}
	}

	// Generate the four corners
	for (int k = 0; k < 4; k++)
	{
		for (int i = 0; i < BLOCK_HEIGHT; i++)
		{
			for (int j = 0; j < BLOCK_WIDTH; j++)
			{
				uint8_t r = corners[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 2];
				uint8_t g = corners[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 1];
				uint8_t b = corners[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j];
				uint32_t pixel = (r << 16) | (g << 8) | b;
				if (pixel) framebuffer[BACKGROUND_WIDTH * (i + corners[k].y_position) + (j + corners[k].x_position)] = pixel; // access the location in the framebuffer that we want to overwrite. 
			}
		}
	}
}