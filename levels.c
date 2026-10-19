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

/*----- Function: level_build_grid-----
v
 PURPOSE: translates our grid into the tile types for level making

 INPUT: level - a level array to build
        grid - the grid to fill

 OUTPUT: None
*/
void level_build_grid(const Level *level, Tile grid[GRID_HEIGHT][GRID_WIDTH]) {

}

