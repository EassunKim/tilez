#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"

/*player directions*/
#define NORTH 0
#define EAST  1
#define SOUTH 2
#define WEST 3

/* PLAYER
* purpose: the struct we will use for both the player and the 'ally' entity
*
* x: the x coordinate of the player
* y: the y coordinate of the player
* facing: the direction the player is facing (0-3, see above)
*/
typedef struct {
    UINT8 x; 
    UINT8 y;
    UINT8 facing;
} Player;

/* Declartions for .c*/
void player_set_position(Player* player, UINT8 x, UINT8 y);
void player_forward_cell(Player* player, INT16* next_x, INT16* next_y);
void player_rotate_right(Player* player);
void player_rotate_left(Player* player);
void player_turn_around(Player* player);
void player_init(Player* player, UINT8 x, UINT8 y, UINT8 facing);

#endif