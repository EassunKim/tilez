
/*------------------------------------------------------------
 * File: renderer.h
 *
 * PURPOSE: Declares the rendering functions used to draw
 *          the TileZ game board, tiles, walls, and player
 *          on the Atari ST monochrome screen.
 *
 * Team Members: Mohamad, Ujjwal, Matthew
 * Author: Mohamad
 *------------------------------------------------------------*/

#ifndef RENDERER_H
#define RENDERER_H

#include "types.h"
#include "player.h"
#include "tile.h"

/*----- Function: render_tile -----
 *
 * PURPOSE: Draws a tile based on its type and wall flags.
 *
 * INPUT: tile - pointer to the tile to draw.
 *        base - pointer to the screen buffer.
 *        row - top screen row of the tile.
 *        col - left screen column of the tile.
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: The tile is within the screen boundaries.
 */
void render_tile(const Tile *tile, UINT8 *base, int row, int col);

/*----- Function: render_walls -----
 *
 * PURPOSE: Draws the walls surrounding a tile.
 *
 * INPUT: tile - pointer to the tile containing wall flags.
 *        base - pointer to the screen buffer.
 *        row - top screen row of the tile.
 *        col - left screen column of the tile.
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: The tile and wall flags are valid.
 */
void render_walls(const Tile *tile, UINT8 *base, int row, int col);

/*----- Function: render_board -----
 *
 * PURPOSE: Draws all tiles on the game board.
 *
 * INPUT: board - two-dimensional array of tiles.
 *        base - pointer to the screen buffer.
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: The board dimensions match GRID_HEIGHT
 *              and GRID_WIDTH.
 */
void render_board(const Tile board[GRID_HEIGHT][GRID_WIDTH],
                  UINT8 *base);

/*----- Function: render_player -----
 *
 * PURPOSE: Draws the player bitmap based on its position
 *          and facing direction.
 *
 * INPUT: player - pointer to the player to draw.
 *        base - pointer to the screen buffer.
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: Player position and direction are valid.
 */
void render_player(const Player *player, UINT8 *base);

#endif
