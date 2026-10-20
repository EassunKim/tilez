/*------------------------------------------------------------
 * File: renderer.c
 *
 * PURPOSE: Draws the TileZ game board, tiles, walls, and
 *          player sprites on the Atari ST monochrome screen.
 *
 * Team Members: Mohamad, Ujjwal, Matthew
 * Author: Mohamad
 *------------------------------------------------------------*/

#include "renderer.h"
#include "raster.h"

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

/* Player bitmap facing north */
static const UINT8 player_north[8] = {
    0x3C, 0x42, 0x99, 0xA5,
    0xBD, 0x81, 0x42, 0x3C
};

/* Player bitmap facing east */
static const UINT8 player_east[8] = {
    0x3C, 0x42, 0x81, 0x9D,
    0xA3, 0x9D, 0x42, 0x3C
};

/* Player bitmap facing south */
static const UINT8 player_south[8] = {
    0x3C, 0x42, 0x81, 0xBD,
    0xA5, 0x99, 0x42, 0x3C
};

/* Player bitmap facing west */
static const UINT8 player_west[8] = {
    0x3C, 0x42, 0x81, 0xB9,
    0xC5, 0xB9, 0x42, 0x3C
};

/*----- Function: render_tile -----
 *
 * PURPOSE: Draws one board tile based on its type
 *          and draws any walls surrounding the tile.
 *
 * INPUT: tile - pointer to the tile to draw.
 *        base - pointer to the screen buffer.
 *        row - top screen row of the tile.
 *        col - left screen column of the tile.
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: The screen buffer is valid and the
 *              tile is within the visible screen.
 */
void render_tile(const Tile *tile, UINT8 *base, int row, int col)
{
    if (tile->type == T_VOID)
    {
        return;
    }

    plot_rectangle((UINT32 *)base, row, col,
                   TILE_HEIGHT, TILE_WIDTH);

    if (tile->type == T_FIRE)
    {
        plot_triangle((UINT32 *)base, row + 8,
                      col + 10, 12, 16, 0);
    }
    else if (tile->type == T_SAFE)
    {
        plot_square((UINT32 *)base, row + 10,
                    col + 10, 12);
    }
    else if (tile->type == T_GOAL)
    {
        plot_square((UINT32 *)base, row + 8,
                    col + 8, 16);
    }
    else if (tile->type == T_SLIP)
    {
        plot_line((UINT32 *)base, row + 8, col + 8,
                  row + 16, col + 16);
        plot_line((UINT32 *)base, row + 16, col + 16,
                  row + 8, col + 24);
    }

    render_walls(tile, base, row, col);
}

/*----- Function: render_walls -----
 *
 * PURPOSE: Draws the walls surrounding a tile based
 *          on the tile's wall flags.
 *
 * INPUT: tile - pointer to the tile containing wall flags.
 *        base - pointer to the screen buffer.
 *        row - top screen row of the tile.
 *        col - left screen column of the tile.
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: The wall flags are valid and the tile
 *              is within the visible screen.
 */
void render_walls(const Tile *tile, UINT8 *base, int row, int col)
{
    if (tile->walls & WALL_NORTH)
    {
        plot_horizontal_line((UINT32 *)base,
                             row, col, TILE_WIDTH);
    }

    if (tile->walls & WALL_EAST)
    {
        plot_vertical_line((UINT32 *)base, row,
                           col + TILE_WIDTH - 1, TILE_HEIGHT);
    }

    if (tile->walls & WALL_SOUTH)
    {
        plot_horizontal_line((UINT32 *)base,
                             row + TILE_HEIGHT - 1,
                             col, TILE_WIDTH);
    }

    if (tile->walls & WALL_WEST)
    {
        plot_vertical_line((UINT32 *)base,
                           row, col, TILE_HEIGHT);
    }
}

/*----- Function: render_board -----
 *
 * PURPOSE: Draws every tile in the game board.
 *
 * INPUT: board - two-dimensional array of game tiles.
 *        base - pointer to the screen buffer.
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: The board has GRID_HEIGHT rows and
 *              GRID_WIDTH columns. Each tile is 32x32
 *              pixels and the board begins at (40,160).
 */
void render_board(const Tile board[GRID_HEIGHT][GRID_WIDTH],
                  UINT8 *base)
{
    int row, col;

    for (row = 0; row < GRID_HEIGHT; row++)
    {
        for (col = 0; col < GRID_WIDTH; col++)
        {
            render_tile(&board[row][col], base,
                        40 + row * TILE_HEIGHT,
                        160 + col * TILE_WIDTH);
        }
    }
}

/*----- Function: render_player -----
 *
 * PURPOSE: Draws an 8x8 player bitmap at the player's
 *          board position and facing direction.
 *
 * INPUT: player - pointer to the player to draw.
 *        base - pointer to the screen buffer.
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: Player coordinates and facing direction
 *              are valid. The screen uses 640x400
 *              monochrome graphics.
 */
void render_player(const Player *player, UINT8 *base)
{
    int row, col;

    row = 40 + player->y * TILE_HEIGHT;
    col = 160 + player->x * TILE_WIDTH;

    if (player->facing == NORTH)
    {
        plot_8bit_bitmap(base, row + 12, col + 12,
                         player_north, 8);
    }
    else if (player->facing == EAST)
    {
        plot_8bit_bitmap(base, row + 12, col + 12,
                         player_east, 8);
    }
    else if (player->facing == SOUTH)
    {
        plot_8bit_bitmap(base, row + 12, col + 12,
                         player_south, 8);
    }
    else if (player->facing == WEST)
    {
        plot_8bit_bitmap(base, row + 12, col + 12,
                         player_west, 8);
    }
}
