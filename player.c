#include "player.h"

/* PLAYER_INIT
* purpose: initializes a player struct with set position and facing values
*/
void player_init(Player* player, UINT8 x, UINT8 y, UINT8 facing) {
    player->x = x;
    player->y = y;
    player->facing = facing;
}

/* ROTATE_LEFT
* purpose: rotates the player left of current heading
*/
void player_rotate_left(Player* player) {
    player->facing = (player->facing + 3) % 4;
}

/* ROTATE_RIGHT
* purpose: rotates the player right of current heading
*/
void player_rotate_right(Player* player) {
    player->facing = (player->facing + 1) % 4;
}

//PLAYER MOVEMENT

/* player_forward_cell
* purpose: returns the cell infront of the player. This is important due to the inclusion of slip and wall tiles
*
* next_x and y are meant to be pointers to return the two values on the atari st
*/
void player_forward_cell(Player* player, UINT8* next_x, UINT8* next_y) {
    int x_directs[4] = {0,1,0,-1}; // the value that next_x should be incremented by based on facing value of player
    int y_directs[4] = {-1,0,1,0}; // same as above but for next_y

    *next_x = player->x + x_directs[player->facing];
    *next_y = player->y + y_directs[player->facing];
}

/* player_set_position
* purpose: sets the player to a new position. I chose this instead of "moving" the player 1 or two tiles because it makes
*          more sense for slip tiles we will implament
*/
void player_set_position(Player* player, UINT8 x, UINT8 y) {
    player->x = x;
    player->y = y;
}