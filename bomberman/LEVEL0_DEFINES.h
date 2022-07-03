#ifndef LEVEL_0_H
#define LEVEL_0_H

#include "../minifb/include/MiniFB_enums.h"
#include "../minifb/include/MiniFB.h"
#include "../FreeImage/Dist/x64/FreeImage.h"
#include "BLOCK.h"
#include "USER.h"

#define BACKGROUND_LOC "assets/airbg.png"
#define CORNER_BLOCK_LOC "assets/air_corner.png"
#define KEY_LOC "assets/air_key.png"
#define TILE_LOC "assets/air_tile_square_border.png"
#define WALL_LOC "assets/air_wall.png"
#define CRATE_LOC "assets/crate.png"
#define COLLISION_OFFSET 58

/*

Window data and framebuffer data should be passed.
This function will hyjack the current window and start placing down map data into the buffer.

Firts argument should be casted to a struct mfb_window before usage.

*/
void level0_init(struct mfb_window*, uint32_t*);

/*

Wrapper function to generate or regenerate the walls and corners of the level.
This takes in the current window, the buffer, and the arrays that contain the block data for each segment.

*/
void generate_walls_and_corners(struct mfb_window*, uint32_t*, Block* wallTop, Block* wallBottom, Block* wallRight, Block* wallLeft, Block* corners);


/*

Wrapper fucntions to generate or regenerate the tiles of the level. 
This takes in the current window, the buffer, and the arrays that containt he block data for each segment.

*/
void generate_tiles(struct mfb_window*, uint32_t*, Block*, Block*, Block*, Block*);

/*
Keyboard callback for player move functions.
Information on this callback function is taken from the Github documentation. 
First parameter takes in the window pointer. 
The second one would be the button pressed.
The third one would be the modifier key pressed.
The fourth one checks if it is currently pressed.
*/
void level0_keyboard_callback(struct mfb_window*, mfb_key, mfb_key_mod, bool);
void generate_player(struct mfb_window*, uint32_t*);
void generate_collision_data(Block*, Block*, Block*, Block*, Block*, int*, int*);
#endif // LEVEL_0_H
