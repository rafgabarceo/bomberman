#ifndef BLOCK_H
#define BLOCK_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define WALL_WIDTH 56
#define WALL_HEIGHT 56
#define BLOCK_HEIGHT 56
#define BLOCK_WIDTH 56

/*

	ENUM DEFINITION:
		Air : Can be passed through. Convertable into explosion block.
		Enemy : Cannot be passed through. Convertable into explosion block. Damages the player when stepped on. 
		Breakable : Cannot be passed through. Convertable into explosion block. 
		Wall : Cannot be passed through. 
		Border : Cannot be passed through. Must be auto-generated at the border of the window.
		Explosion : Cannot be passed through. Is capable of converting player, enemies, etc. into air blocks.
*/
typedef enum BLOCK_TYPES {
	AIR_BLOCK = 0,
	ENEMY_BLOCK = 1,
	BREAKABLE_BLOCK = 2,
	WALL_BLOCK = 3,
	BORDER_BLOCK = 4,
	EXPLOSION_BLOCK = 5
} block_type;

/*

	ENUM Definition:
		Broken : broken
		Present : present

*/
typedef enum BLOCK_STATE {
	BROKEN = 0,
	PRESENT = 1
} block_state;

typedef struct Block {
	uint8_t* imageData;
	int x_position;
	int y_position; 
	int type;
	int state;
} Block;

void setBlockXPosition(Block*, int);
void setBlockYPosition(Block*, int);

/*

	ENUM Definition:
		Broken : broken
		Present : present

*/
void setBlockState(Block*, block_state);

/*

	ENUM DEFINITION:
		Air : Can be passed through. Convertable into explosion block.
		Enemy : Cannot be passed through. Convertable into explosion block. Damages the player when stepped on.
		Breakable : Cannot be passed through. Convertable into explosion block.
		Wall : Cannot be passed through.
		Border : Cannot be passed through. Must be auto-generated at the border of the window.
		Explosion : Cannot be passed through. Is capable of converting player, enemies, etc. into air blocks.
*/
void setBlockType(Block*, block_type);

#endif