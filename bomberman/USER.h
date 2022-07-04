/*
Will contain the defines, constants, and functions that will concern itself with the user model. 
This includes, but are not limited to:
	Current level
	Current score
	Current timer
	Current lives

Information such as this should be passed to each respective level function.
*/
#ifndef USER_H
#define USER_H
#include <stdint.h>
#include "BLOCK.h"

#define USER_LOC "assets/user_black.png"
#define USER_MOVEMENT_L0 56

typedef enum collision {
	OPEN = 0,
	CLOSED = 1
} collision;

typedef struct USER_DAT {
	uint8_t* imageData;
	int user_pos_x_prev;
	int user_pos_y_prev;
	int user_pos_x;
	int user_pos_y;
	int user_score;
	struct mfb_timer* user_timer;
	int bombsDrop;
	Block* bombs;
	collision collision_status_top;
	collision collision_status_bottom;
	collision collision_status_right;
	collision collision_status_left;
} USER_DAT;

#endif // USER_H