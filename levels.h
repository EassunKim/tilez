#ifndef LEVELS_H
#define LEVELS_H

#include "types.h"
#include "tile.h"
#include "player.h"

#define LEVELS 1

//wall
// Defines the tile a wall is on and the side of that wall it is facing/blocking
typedef struct {
    UINT8 x;
    UINT8 y;
    UINT8 facing;
} wallInfo;

//Level
// Defines a full level layout including the spawns, tile layout and walls
typedef struct {
    char *rows[GRID_HEIGHT];
    wallInfo *walls;
    Player player_spawn;
    Player ally_spawn;
} Level;

#endif