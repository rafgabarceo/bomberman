#include "LEVEL0_DEFINES.h"
#include "WINDOW_CONSTANTS.h"
#include "../minifb/include/Minifb.h"
#include "../FreeImage/Dist/x64/FreeImage.h"
#include "../minifb/include/MiniFB_enums.h"

void level0_init(struct mfb_window* window, uint32_t* framebuffer)
{
	mfb_set_keyboard_callback(window, level0_keyboard_callback);
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

	FIBITMAP* tiles_FBM = FreeImage_Load(FIF_PNG, TILE_LOC, PNG_DEFAULT);
	FreeImage_FlipVertical(tiles_FBM);
	uint8_t* tileBits = FreeImage_GetBits(tiles_FBM);

	FIBITMAP* player_FBM = FreeImage_Load(FIF_PNG, USER_LOC, PNG_DEFAULT);
	FreeImage_FlipVertical(player_FBM);
	uint8_t* playerBits = FreeImage_GetBits(player_FBM);

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

	// Generate the tile data
	Block tiles[10]; 
	for (int i = 0; i < 10; i++)
	{
		tiles[i] = (Block){
			.imageData = tileBits,
			.state = PRESENT,
			.type = WALL_BLOCK,
			.x_position = 112 + 112*i,
			.y_position = 56*2
		};
	}

	Block tiles1[10];
	for (int i = 0; i < 10; i++)
	{
		tiles1[i] = (Block){
			.imageData = tileBits,
			.state = PRESENT,
			.type = WALL_BLOCK,
			.x_position = 112 + 112 * i,
			.y_position = 56 * 2 + 112
		};
	}

	Block tiles2[10];
	for (int i = 0; i < 10; i++)
	{
		tiles2[i] = (Block){
			.imageData = tileBits,
			.state = PRESENT,
			.type = WALL_BLOCK,
			.x_position = 112 + 112 * i,
			.y_position = 56 * 3 + 168
		};
	}

	Block tiles3[10];
	for (int i = 0; i < 10; i++)
	{
		tiles3[i] = (Block){
			.imageData = tileBits,
			.state = PRESENT,
			.type = WALL_BLOCK,
			.x_position = 112 + 112 * i,
			.y_position = 56 * 4 + 224
		};
	}

	while (1)
	{

		int state = mfb_update_ex(window, (void*)framebuffer, WINDOW_WIDTH, WINDOW_HEIGHT);
		
		// Load the background into the page. 
		for (int i = 0; i < (BACKGROUND_HEIGHT * BACKGROUND_WIDTH * 4); i += 4)
		{
			framebuffer[i / 4] = (backgroundBits[i + 2] << 16) | (backgroundBits[i + 1] << 8) | (backgroundBits[i]);
		}
		
		generate_walls_and_corners(window, framebuffer, wallsTop, wallsBottom, wallsRight, wallsLeft, corners);
		generate_tiles(window, framebuffer, tiles, tiles1, tiles2, tiles3);
		generate_player(window, framebuffer);

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

void generate_tiles(struct mfb_window* window, uint32_t* framebuffer, Block* tiles, Block* tiles1, Block* tiles2, Block* tiles3)
{
	USER_DAT* user = (USER_DAT*)mfb_get_user_data(window); // Get the user object.
	// Generate the tiles
	for (int k = 0; k < 10; k++)
	{
		for (int i = 0; i < BLOCK_HEIGHT; i++)
		{
			for (int j = 0; j < BLOCK_WIDTH; j++)
			{
				uint8_t r = tiles[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 3];
				uint8_t g = tiles[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 2];
				uint8_t b = tiles[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 1];
				uint8_t t = tiles[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j];
				uint32_t pixel = (r << 24) | (g << 16) | (b << 8) | t;
				if (pixel) framebuffer[BACKGROUND_WIDTH * (i + tiles[k].y_position) + (j + tiles[k].x_position)] = pixel; // access the location in the framebuffer that we want to overwrite. 

				r = tiles1[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 3];
				g = tiles1[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 2];
				b = tiles1[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 1];
				t = tiles1[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j];
				pixel = (r << 24) | (g << 16) | (b << 8) | t;
				if (pixel) framebuffer[BACKGROUND_WIDTH * (i + tiles1[k].y_position) + (j + tiles1[k].x_position)] = pixel; // access the location in the framebuffer that we want to overwrite. 
				r = tiles2[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 3];
				g = tiles2[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 2];
				b = tiles2[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 1];
				t = tiles2[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j];
				pixel = (r << 24) | (g << 16) | (b << 8) | t;
				if (pixel) framebuffer[BACKGROUND_WIDTH * (i + tiles2[k].y_position) + (j + tiles2[k].x_position)] = pixel; // access the location in the framebuffer that we want to overwrite. 

				r = tiles3[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 3];
				g = tiles3[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 2];
				b = tiles3[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 1];
				t = tiles3[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j];
				pixel = (r << 24) | (g << 16) | (b << 8) | t;
				if (pixel) framebuffer[BACKGROUND_WIDTH * (i + tiles3[k].y_position) + (j + tiles3[k].x_position)] = pixel; // access the location in the framebuffer that we want to overwrite. 
			}
		}
	}
}

void level0_keyboard_callback(struct mfb_window* window, mfb_key key, mfb_key_mod mod, bool isPressed)
{
	USER_DAT* user = (USER_DAT*)mfb_get_user_data(window);
	// Determine the collision properties for the next movement.

	user->user_pos_x_prev = user->user_pos_x;
	user->user_pos_y_prev = user->user_pos_y;

	if (isPressed)
	{
		if (key == KB_KEY_RIGHT)
		{
			user->user_pos_x += USER_MOVEMENT_L0;
		}
		else if (key == KB_KEY_LEFT)
		{
			user->user_pos_x -= USER_MOVEMENT_L0;
		}
		else if (key == KB_KEY_DOWN)
		{
			user->user_pos_y += USER_MOVEMENT_L0;
		}
		else if (key == KB_KEY_UP)
		{
				user->user_pos_y -= USER_MOVEMENT_L0;
		}
	}

	// Check x, y collision on tiles
	if ((((user->user_pos_y) / 56) % 2 == 0) && (((user->user_pos_x) / 56) % 2 == 0)) // Collision detected.
	{
		user->user_pos_y = user->user_pos_y_prev;
		user->user_pos_x = user->user_pos_x_prev;
	}
	else if ((user->user_pos_x == 0) || (user->user_pos_x == 1232))
	{
		user->user_pos_x = user->user_pos_x_prev;
	}
	else if ((user->user_pos_y == 0) || (user->user_pos_y == 560))
	{
		user->user_pos_y = user->user_pos_y_prev;
	}
}

void generate_player(struct mfb_window* window, uint32_t* framebuffer)
{
	USER_DAT* user = (USER_DAT*)mfb_get_user_data(window);
	uint32_t borderBlockColor = 0xfbe3ae;

	for (int i = 0; i < BLOCK_HEIGHT; i++)
	{
		for (int j = 0; j < BLOCK_WIDTH; j++)
		{
			uint8_t r = user->imageData[BLOCK_WIDTH * 3 * i + 3 * j + 2];
			uint8_t g = user->imageData[BLOCK_WIDTH * 3 * i + 3 * j + 1];
			uint8_t b = user->imageData[BLOCK_WIDTH * 3 * i + 3 * j];
			uint32_t pixel = (r << 16) | (g << 8) | b;
			if (pixel) framebuffer[BACKGROUND_WIDTH * (i + user->user_pos_y) + (j + user->user_pos_x)] = pixel; // access the location in the framebuffer that we want to overwrite. 
		}
	}



	printf("x: %i, y: %i \n", user->user_pos_x, user->user_pos_y);
}

void generate_collision_data(Block* wallsTop, Block*wallsBottom, Block*wallsRight, Block* wallsLeft, Block* tiles, int* collisionX, int* collisionY)
{
	/*
		Generate x, y positions for the walls
		Generate x, y positions for the tiles
	*/
}