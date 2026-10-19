#ifndef LEVELS_H
#define LEVELS_H

#include "types.h"
#include "tile.h"
#include "player.h"

#define LEVELS 1
#define GRID_WIDTH  10
#define GRID_HEIGHT 10


/*wall
Defines the tile a wall is on and the side of that wall it is facing/blocking
*/
typedef struct {
    UINT8 x;
    UINT8 y;
    UINT8 side;
} wallInfo;

/*Level
Defines a full level layout including the spawns, tile layout and walls
*/
typedef struct {
    const char *rows[GRID_HEIGHT];
    const wallInfo *walls;
    const UINT8 wall_count;
    Player player_spawn;
    Player ally_spawn;
} Level;

/*declarations*/
static UINT8 char_to_tile_type(char c);
const Level *level_get(UINT8 index);
void level_build_grid(const Level *level, Tile grid[GRID_HEIGHT][GRID_WIDTH]);



#endif