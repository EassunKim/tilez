#ifndef RENDERER_H
#define RENDERER_H

#include "types.h"
#include "player.h"
#include "tile.h"

void render_tile(const Tile *tile, UINT8 *base, int row, int col);
void render_walls(const Tile *tile, UINT8 *base, int row, int col);
void render_board(const Tile board[GRID_HEIGHT][GRID_WIDTH],
                  UINT8 *base);

void render_player(const Player *player, UINT8 *base);

#endif