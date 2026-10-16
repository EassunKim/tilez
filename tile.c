#include "tile.h"

/*----- Function: tile_init -----
v
 PURPOSE: initializes a tile struct with type and wall value

 INPUT: 

 OUTPUT: None

*/
void tile_init(Tile* tile, UINT8 type, UINT8 walls) {
    tile->type = type;
    tile->walls = walls;
}

/*----- Function: check_walls -----
v
 PURPOSE: Checks if a wall is present in a certain direction of a tile. will be used for both player movement and rendering

 INPUT: side - a direction (0-4)

 OUTPUT: TRUE if a wall is present, FALSE if not

*/
BOOL tile_check_walls(Tile* tile, UINT8 side) {
    return (tile->walls & (1 << side)) != 0;
}

/*----- Function: tile_add_wall -----
v
 PURPOSE: add a wall on a certain side of a tile

 INPUT: side - a direction (0-4)

 OUTPUT: None

*/
void tile_add_wall(Tile* tile, UINT8 side) {
    tile->walls |= (1 << side);
}

/*----- Function: tile_is_valid-----
v
 PURPOSE: checks if a tils is a valid place to stand

 OUTPUT: TRUE for a tile that isn't a void tile

*/
BOOL tile_is_valid(Tile* tile) {
    return tile->type != T_VOID;
}