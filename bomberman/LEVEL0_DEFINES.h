#ifndef LEVEL_0_H
#define LEVEL_0_H

#include "../minifb/include/MiniFB_enums.h"
#include "../minifb/include/MiniFB.h"
#include "../FreeImage/Dist/x64/FreeImage.h"
#include "BLOCK.h"

#define BACKGROUND_LOC "assets/airbg.png"
#define CORNER_BLOCK_LOC "assets/air_corner.png"
#define KEY_LOC "assets/air_key.png"
#define TILE_LOC "assets/air_tiles.png"
#define WALL_LOC "assets/air_wall.png"
#define CRATE_LOC "assets/crate.png"

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

#endif // LEVEL_0_H
