#include "BLOCK.h"

void setBlockXPosition(Block* block, int x)
{
	block->x_position = x;
}

void setBlockYPosition(Block* block, int y)
{
	block->y_position = y;
}

void setBlockState(Block* block, block_state state)
{
	block->state = state;
}

void setBlockType(Block* block, block_type type)
{
	block->type = type;
}