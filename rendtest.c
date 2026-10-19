
#include <stdio.h>
#include <string.h>
#include "renderer.h"

#define N 32000

static UINT8 screen[N + 16];

static int is_set(int row, int col)
{
    return (screen[8 + row * 80 + col / 8]
            & (0x80 >> (col % 8))) != 0;
}

static void reset_screen(void)
{
    memset(screen, 0, sizeof(screen));
}

int main(void)
{
    UINT8 *base = screen + 8;
    Tile tile;
    Tile board[GRID_HEIGHT][GRID_WIDTH];
    int r, c;
    Player player;

    /* TEST 1: Normal tile */
    reset_screen();

    tile.type = T_NORMAL;
    tile.walls = 0;

    render_tile(&tile, base, 40, 160);

    if (is_set(40, 160) && is_set(40, 191) &&
        is_set(71, 160) && is_set(71, 191))
    {
        printf("PASS normal tile\n");
    }
    else
    {
        printf("FAIL normal tile\n");
    }

    /* TEST 2: Fire tile */
    reset_screen();

    tile.type = T_FIRE;
    tile.walls = 0;

    render_tile(&tile, base, 40, 160);

    if (is_set(48, 170))
    {
        printf("PASS fire tile\n");
    }
    else
    {
        printf("FAIL fire tile\n");
    }

    /* TEST 3: Safe tile */
    reset_screen();

    tile.type = T_SAFE;
    tile.walls = 0;

    render_tile(&tile, base, 40, 160);

    if (is_set(50, 170) && is_set(50, 181) &&
        is_set(61, 170) && is_set(61, 181))
    {
        printf("PASS safe tile\n");
    }
    else
    {
        printf("FAIL safe tile\n");
    }

    /* TEST 4: Goal tile */
    reset_screen();

    tile.type = T_GOAL;
    tile.walls = 0;

    render_tile(&tile, base, 40, 160);

    if (is_set(48, 168) && is_set(48, 183) &&
        is_set(63, 168) && is_set(63, 183))
    {
        printf("PASS goal tile\n");
    }
    else
    {
        printf("FAIL goal tile\n");
    }

    /* TEST 5: Slip tile */
    reset_screen();

    tile.type = T_SLIP;
    tile.walls = 0;

    render_tile(&tile, base, 40, 160);

    if (is_set(48, 168) &&
        is_set(56, 176) &&
        is_set(48, 184))
    {
        printf("PASS slip tile\n");
    }
    else
    {
        printf("FAIL slip tile\n");
    }

    /* TEST 6: North and East walls */
    reset_screen();

    tile.type = T_NORMAL;
    tile.walls = WALL_NORTH | WALL_EAST;

    render_walls(&tile, base, 40, 160);

    if (is_set(40, 175) &&
        is_set(55, 191) &&
        !is_set(71, 175) &&
        !is_set(55, 160))
    {
        printf("PASS north and east walls\n");
    }
    else
    {
        printf("FAIL north and east walls\n");
    }

    /* TEST 7: Entire board */
    reset_screen();

    for (r = 0; r < GRID_HEIGHT; r++)
    {
        for (c = 0; c < GRID_WIDTH; c++)
        {
            board[r][c].type = T_NORMAL;
            board[r][c].walls = 0;
        }
    }

    render_board(board, base);

    if (is_set(40, 160) &&
        is_set(40, 479) &&
        is_set(359, 160) &&
        is_set(359, 479))
    {
        printf("PASS board\n");
    }
    else
    {
        printf("FAIL board\n");
    }

    /* TEST 8: Player facing North */
reset_screen();

player.x = 3;
player.y = 4;
player.facing = NORTH;

render_player(&player, base);

if (is_set(176, 264) &&
    is_set(191, 279) &&
    is_set(172, 272))
{
    printf("PASS player north\n");
}
else
{
    printf("FAIL player north\n");
}


/* TEST 9: Player facing East */
reset_screen();

player.x = 3;
player.y = 4;
player.facing = EAST;

render_player(&player, base);

if (is_set(184, 279) &&
    is_set(184, 283) &&
    !is_set(172, 272))
{
    printf("PASS player east\n");
}
else
{
    printf("FAIL player east\n");
}



/* TEST 10: Player facing South */
reset_screen();

player.x = 3;
player.y = 4;
player.facing = SOUTH;

render_player(&player, base);

if (is_set(191, 272) &&
    is_set(195, 272) &&
    !is_set(172, 272))
{
    printf("PASS player south\n");
}
else
{
    printf("FAIL player south\n");
}

/* TEST 11: Player facing West */
reset_screen();

player.x = 3;
player.y = 4;
player.facing = WEST;

render_player(&player, base);

if (is_set(184, 260) &&
    is_set(184, 264) &&
    !is_set(184, 283))
{
    printf("PASS player west\n");
}
else
{
    printf("FAIL player west\n");
}


    return 0;
}
