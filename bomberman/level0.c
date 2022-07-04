#include "LEVEL0_DEFINES.h"
#include "WINDOW_CONSTANTS.h"
#include "../minifb/include/Minifb.h"
#include "../FreeImage/Dist/x64/FreeImage.h"
#include "../minifb/include/MiniFB_enums.h"

void level0_init(struct mfb_window* window, uint32_t* framebuffer)
{
	struct mfb_timer* ticks = mfb_timer_create();

	mfb_set_keyboard_callback(window, level0_keyboard_callback);
	USER_DAT* user = (USER_DAT*)mfb_get_user_data(window);
	user->user_timer = ticks; // save the timer into user data to be picked up from the callback.

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

	FIBITMAP* bomb_FBM = FreeImage_Load(FIF_PNG, BOMB_LOC, PNG_DEFAULT);
	FreeImage_FlipVertical(bomb_FBM);
	uint8_t* bombBits = FreeImage_GetBits(bomb_FBM);

	FIBITMAP* tiles_FBM = FreeImage_Load(FIF_PNG, TILE_LOC, PNG_DEFAULT);
	FreeImage_FlipVertical(tiles_FBM);
	uint8_t* tileBits = FreeImage_GetBits(tiles_FBM);

	FIBITMAP* player_FBM = FreeImage_Load(FIF_PNG, USER_LOC, PNG_DEFAULT);
	FreeImage_FlipVertical(player_FBM);
	uint8_t* playerBits = FreeImage_GetBits(player_FBM);

	FIBITMAP* crate_FBM = FreeImage_Load(FIF_PNG, CRATE_BLOCK_LOC, PNG_DEFAULT);
	FreeImage_FlipVertical(crate_FBM);
	uint8_t* crateBits = FreeImage_GetBits(crate_FBM);

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

	Block bombs[BOMB_AMMO];

	for (int i = 0; i < BOMB_AMMO; i++)
	{
		bombs[i] = (Block){
			.imageData = bombBits,
			.state = PRESENT,
			.type = BOMB_BLOCK,
			.x_position = 56+(5*i),
			.y_position = BACKGROUND_HEIGHT + 23
		};
	}

	user->bombs = bombs;

	// Generate the metadata for the corner blocks
	Block corners[4];
	for (int i = 0; i < 4; i++)
	{
		corners[i] = (Block){
			.imageData = cornerBits,
			.state = PRESENT,
			.type = WALL_BLOCK,
			.x_position = cornersX[i],
			.y_position = cornersY[i],
			.timerIsInitialized = false
		};
	}

	static int cratesX[44];
	static int cratesY[44];


	// Generate crate data
	Block crates[CRATE_COUNT];
	for (int i = 0; i < CRATE_COUNT; i++)
	{
		crates[i] = (Block){
		.imageData = crateBits,
		.state = PRESENT,
		.type = BREAKABLE_BLOCK,
		.x_position = CRATE_COORDINATE_DATA[2*i],
		.y_position = CRATE_COORDINATE_DATA[2*i + 1]
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

		/*
		
			NOTE: THIS ROUTINE CAUSES A MEMORY LEAK DUE TO A FREE() HEAP ERROR IN MINIFB.
		
		*/
		if (user->bombsDrop == 0) // Special case to avoid -1 array access.
		{
			if (user->bombs[0].timerIsInitialized == true)
			{
				if (mfb_timer_now(user->bombs[0].blocktimer) > 3.00)
				{
					user->bombs[0].state = USED; // Will no longer be generated.
					// mfb_timer_destroy(user->bombs[0].blocktimer); // Destroy the timer.
				}
			}
		}
		else if (user->bombs[user->bombsDrop - 1].timerIsInitialized == true) // subtract by one to delete the previous bomb place
		{
			if (mfb_timer_now(user->bombs[user->bombsDrop - 1].blocktimer) > 3.00)
			{
				user->bombs[user->bombsDrop - 1].state = USED; // Will no longer be generated.
				// mfb_timer_destroy(user->bombs[user->bombsDrop - 1].blocktimer); // Destroy the timer. Causes heap crash
			}
		}
		// printf("Delta: %f \n", mfb_timer_now(ticks));

		// Load the background into the page. 
		for (int i = 0; i < (BACKGROUND_HEIGHT * BACKGROUND_WIDTH * 4); i += 4)
		{
			framebuffer[i / 4] = (backgroundBits[i + 2] << 16) | (backgroundBits[i + 1] << 8) | (backgroundBits[i]);
		}

		for (int i = (BACKGROUND_HEIGHT * BACKGROUND_WIDTH * 4); i < (WINDOW_HEIGHT * WINDOW_WIDTH * 4); i += 4)
		{
			framebuffer[i / 4] = 0xFFFFFF;
		}

		generate_crates(window, framebuffer, crates);
		generate_walls_and_corners(window, framebuffer, wallsTop, wallsBottom, wallsRight, wallsLeft, corners);
		generate_tiles(window, framebuffer, tiles, tiles1, tiles2, tiles3);
		generate_player(window, framebuffer);
		generate_bomb(window, framebuffer, bombs);

		if (user->bombsDrop == BOMB_AMMO-1)
		{
			printf("Out of bombs. Exiting.");
			break; // exit to game
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
		else if (key == KB_KEY_A) // place da bomb
		{

			// Initiliaze bomb countdown
			struct mfb_timer* bombTime = mfb_timer_create();
			user->bombs[user->bombsDrop].blocktimer = bombTime; // Attach the bomb timer
			user->bombs[user->bombsDrop].timerIsInitialized = true;
			user->bombs[user->bombsDrop].x_position = user->user_pos_x;
			user->bombs[user->bombsDrop].y_position = user->user_pos_y;
			user->bombsDrop++;
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

	// Check if any coordinates hit the crate data.
	for (int i = 0; i < CRATE_COUNT; i++)
	{
		if (user->user_pos_y == CRATE_COORDINATE_DATA[2 * i + 1] && user->user_pos_x == CRATE_COORDINATE_DATA[2 * i])
		{
			user->user_pos_y = user->user_pos_y_prev;
			user->user_pos_x = user->user_pos_x_prev;
		}
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
	// printf("x: %i, y: %i \n", user->user_pos_x, user->user_pos_y);
}

void generate_collision_data(Block* wallsTop, Block*wallsBottom, Block*wallsRight, Block* wallsLeft, Block* tiles, int* collisionX, int* collisionY)
{
	/*
		Generate x, y positions for the walls
		Generate x, y positions for the tiles
	*/
}

void generate_crates(struct mfb_window* mainWindow, uint32_t* framebuffer, Block* crates)
{
	for (int k = 0; k < CRATE_COUNT; k++)
	{
		if (crates[k].state != BROKEN)
		{
			for (int i = 0; i < BLOCK_HEIGHT; i++)
			{
				for (int j = 0; j < BLOCK_WIDTH; j++)
				{
					uint8_t r = crates[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 2];
					uint8_t g = crates[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 1];
					uint8_t b = crates[k].imageData[BLOCK_WIDTH * 3 * i + 3 * j];
					uint32_t pixel = (r << 16) | (g << 8) | b;
					if (pixel) framebuffer[BACKGROUND_WIDTH * (i + crates[k].y_position) + (j + crates[k].x_position)] = pixel; // access the location in the framebuffer that we want to overwrite. 
				}
			}
		}

	}
}
void generate_bomb(struct mfb_window* mainWindow, uint32_t* framebuffer, Block* bomb)
{
	USER_DAT* user = (USER_DAT*)mfb_get_user_data(mainWindow);
	for (int k = 0; k < BOMB_AMMO; k++)
	{
		for (int i = 0; i < BLOCK_HEIGHT; i++)
		{
			for (int j = 0; j < BLOCK_WIDTH; j++)
			{
				uint8_t r = bomb[user->bombsDrop].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 2];
				uint8_t g = bomb[user->bombsDrop].imageData[BLOCK_WIDTH * 3 * i + 3 * j + 1];
				uint8_t b = bomb[user->bombsDrop].imageData[BLOCK_WIDTH * 3 * i + 3 * j];
				uint32_t pixel = (r << 16) | (g << 8) | b;
				if (pixel && user->bombs[k].state == PRESENT) framebuffer[BACKGROUND_WIDTH * (i + user->bombs[k].y_position) + (j + user->bombs[k].x_position)] = pixel; // access the location in the framebuffer that we want to overwrite. 
			}
		}
	}
}