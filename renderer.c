
#include "renderer.h"
#include "raster.h"

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

void render_tile(const Tile *tile, UINT8 *base, int row, int col)
{
    /* Do not draw void tiles */
    if (tile->type == T_VOID)
    {
        return;
    }

    /* Draw the outline of the tile */
    plot_rectangle((UINT32 *)base, row, col, TILE_HEIGHT, TILE_WIDTH);

    /* Draw a fire tile */
    if (tile->type == T_FIRE)
    {
        plot_triangle((UINT32 *)base, row + 8, col + 10, 12, 16, 0);
    }

    /* Draw a safe tile */
    else if (tile->type == T_SAFE)
    {
        plot_square((UINT32 *)base, row + 10, col + 10, 12);
    }

    /* Draw a goal tile */
    else if (tile->type == T_GOAL)
    {
        plot_square((UINT32 *)base, row + 8, col + 8, 16);
    }

    /* Draw a slip tile */
    else if (tile->type == T_SLIP)
    {
        plot_line((UINT32 *)base, row + 8, col + 8,
                  row + 16, col + 16);

        plot_line((UINT32 *)base, row + 16, col + 16,
                  row + 8, col + 24);
    }

    render_walls(tile, base, row, col);

    /* Normal tiles only have the outline */
}


void render_walls(const Tile *tile, UINT8 *base, int row, int col)
{
    if (tile->walls & WALL_NORTH)
    {
        plot_horizontal_line((UINT32 *)base, row, col, TILE_WIDTH);
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
        plot_vertical_line((UINT32 *)base, row, col, TILE_HEIGHT);
    }
}

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


void render_player(const Player *player, UINT8 *base)
{
    int row, col;

    row = 40 + player->y * TILE_HEIGHT;
    col = 160 + player->x * TILE_WIDTH;

    plot_square((UINT32 *)base, row + 8, col + 8, 16);

    if (player->facing == NORTH)
    {
        plot_vertical_line((UINT32 *)base, row + 4, col + 16, 5);
    }
    else if (player->facing == EAST)
    {
        plot_horizontal_line((UINT32 *)base, row + 16, col + 23, 5);
    }
    else if (player->facing == SOUTH)
    {
        plot_vertical_line((UINT32 *)base, row + 23, col + 16, 5);
    }
    else if (player->facing == WEST)
    {
        plot_horizontal_line((UINT32 *)base, row + 16, col + 4, 5);
    }
}

