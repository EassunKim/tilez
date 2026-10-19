#include "player.h"


/*----- Function: player_init -----
* PURPOSE:   initializes a player struct with set position and facing values
*
* INPUT:     x, y - the x and y position of the player.
*            facing - the heading of the player (0-3 as outlined in player.h)
*
* OUTPUT:    None
*/
/* PLAYER_INIT
* purpose: initializes a player struct with set position and facing values
*/
void player_init(Player* player, UINT8 x, UINT8 y, UINT8 facing) {
    player->x = x;
    player->y = y;
    player->facing = facing;
}

/*---------------DIRECTION CHANGE FUNCTIONS --------------------*/

/*----- Function: rotate_left -----
* PURPOSE:   rotates the player left of current heading
*
* INPUT:     player - a Player object whose heading will be updated
*
* OUTPUT: None
*/
void player_rotate_left(Player* player) {
    player->facing = (player->facing + 3) % 4; /*using mod to 'wrap' the value back around to 0-3 if exceeded*/
}

/*----- Function: rotate_right-----
* PURPOSE:   rotates the player right of current heading
*
* INPUT:     player - a Player object whose heading will be updated
*
* OUTPUT:    None
*/
void player_rotate_right(Player* player) {
    player->facing = (player->facing + 1) % 4;
}

/*----- Function: player_turn_around-----
* PURPOSE:   purpose rotates the player 180 degree
*
* INPUT:     player - a Player object whose heading will be updated
*
* OUTPUT:    None
*/
void player_turn_around(Player* player) {
    player->facing = (player->facing + 2) % 4;
}

/*-----------PLAYER MOVEMENT FUNCTIONS-----------*/

/*----- Function: player_forward_cell -----
* PURPOSE:   returns the cell infront of the player. 
*            This is important due to the inclusion of slip and wall tiles
*
* INPUT:    player - a Player object facing some tile
*           next_x - pointer to the x value of the tile in front of the player
*           next_y - pointer to the x value of the tile in front of the player
*
*OUTPUT:    None
*/
void player_forward_cell(Player* player, INT16* next_x, INT16* next_y) {
    int x_directs[4] = {0,1,0,-1}; /* the value that next_x should be incremented by based on facing value of player*/
    int y_directs[4] = {-1,0,1,0}; /* same as above but for next_y*/

    *next_x = player->x + x_directs[player->facing];
    *next_y = player->y + y_directs[player->facing];
}


/*----- Function: player_set_position -----
* PURPOSE:  sets the player to a new position. I chose this instead of "moving" the player 1 or two tiles because it makes
*           more sense for slip tiles we will implament
*
* INPUT:    player - the player to be moved
*           x, y   - the coordinates of the new position
*
* OUTPUT:   None
*
* NOTE:     I chose this instead of "moving" the player 1 or two tiles because it makes
*           more sense for slip tiles we will implament
*/
void player_set_position(Player* player, UINT8 x, UINT8 y) {
    player->x = x;
    player->y = y;
}