#include <stddef.h>
#include "levels.h"

/*level 0 - the testing stage*/

static const wallInfo level0_walls[] = { {3,5, EAST}, {3,4, SOUTH} };

static const Level levels[LEVELS] = {
    {
        {
            "          ",
            "  ......  ",
            "  .F....  ",
            "  ......  ",
            "  ....S.  ",
            "  ..G...  ",
            "  ......  ",
            "          ",
            "          ",
            "          "
        },
        level0_walls, 
        2,
        {3, 4, NORTH}, /*player spawn*/
        {7, 5, SOUTH}  /*ally spawn*/
    }
};

/* functions*/
/*----- Function: char_to_tile_type -----
v
 PURPOSE: translates our grid into the tile types for level making

 INPUT: c - c character that will be translated into a tile

 OUTPUT: an integer that corresponds to the tile type, see TILES in tiles.h

 NOTE:  will return void if a char is not in the legend, but a blank space is what is used as a void in the 
        actual level setup, (see levels above)
*/
static UINT8 char_to_tile_type(char c){
    switch (c)
    {
        case '.': return T_NORMAL;
        case 'F': return T_FIRE;
        case 'S': return T_SLIP;
        case 'G': return T_GOAL;
        default:  return T_VOID;
    }
}

const Level *level_get(UINT8 i) {
    const Level* cur;
    if (i < LEVELS) {
        cur = &levels[i];
        return cur;
    }
    return NULL;
}

/*level building*/

/*----- Function: build_row-----
 PURPOSE:   A helper function for level_build_grid. Given a row of the level layout string, fills in one
            row of the grid

 INPUT:     row - The row string consisting of the tile indicators (F, ., , etc.)
            tiles - the row of tiles to initialize

 OUTPUT:    NONE
*/
static void build_row(const char *row, Tile row_tiles[GRID_WIDTH]) {
    UINT8 x;
    BOOL at_end = FALSE; /* flag to determine when at the end of the row*/

    for (x = 0; x < GRID_WIDTH; x++) {
        if (!at_end && row[x] == '\0') {
            at_end = TRUE;
        }

        /* all walls set to 0, apply_walls will always be called with this function*/
        if (at_end) {
            tile_init(&row_tiles[x], T_VOID, 0);
        }

        else {
            tile_init(&row_tiles[x], char_to_tile_type(row[x]), 0);
        }

    }
}

/*----- Function: apply_walls -----
 PURPOSE:   Helper function for A helper function for level_build_grid. Applys the needed walls for 
            a level
 INPUT:     level - the level used to build the row
            grid - the grid to add walls to

 OUTPUT: NONE
*/
static void apply_walls(const Level *level, Tile grid[GRID_HEIGHT][GRID_WIDTH]) {
    UINT8 i;
    const wallInfo *wall;

    for (i = 0; i < level->wall_count; i++) {
        wall = &level->walls[i];
        tile_add_wall(&grid[wall->y][wall->x], wall->side); 
    }
}

/*----- Function: level_build_grid-----
 PURPOSE: translates our grid into the tile types for level making

 INPUT: lvl - a level array to build
        grid - a 2d array of Tile objects (see tile.h) to initialize

 OUTPUT: None
*/
void level_build_grid(const Level *lvl, Tile grid[GRID_HEIGHT][GRID_WIDTH]) {
    UINT8 y;

    for (y = 0; y<GRID_HEIGHT; y++) {
        build_row(lvl->rows[y], grid[y]);
    }

    apply_walls(lvl, grid);
}

