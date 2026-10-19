#ifndef TILE_H
#define TILE_H

#include "types.h"

/*TILES*/
#define T_VOID 0
#define T_NORMAL 1
#define T_GOAL 2
#define T_SLIP 3
#define T_FIRE 4

/*WALL DIRECTIONS*/
/* A Note on my walls
*  I chose to use a bitmask for walls.
*  This gives us an easy flag for each wall, the ability to check which walls are present (0x03 would be north and east)
*  and allow us to change the flags with a shift, which we saw in class was rather cheap.
*/
#define WALL_NORTH 0x01
#define WALL_EAST  0x02
#define WALL_SOUTH 0x04
#define WALL_WEST  0x08

/* Tile struct
* type  - the tile's type (see 'TILES' above)
* walls - the tile's walls (see 'WALL DIRECTIONS' above)
*/
typedef struct {
    UINT8 type; /*a value to denote the type of tile*/
    UINT8 walls; /* a bitmask of flags that denote what walls are around a tile\*/
} Tile;

/*Declaroins*/
BOOL tile_is_valid(Tile* tile);
void tile_add_wall(Tile* tile, UINT8 side);
BOOL tile_check_walls(Tile* tile, UINT8 side);
void tile_init(Tile* tile, UINT8 type, UINT8 walls);

#endif