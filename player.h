#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"

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


#endif