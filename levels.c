#include <stddef.h>
#include "levels.h"

// test level

static const wallInfo level0_walls[] = { {4,5, WALL_EAST}, {3,4, WALL_SOUTH} };

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
        {3, 4, NORTH}, //player spawn
        {4, 5, SOUTH}  //ally spawn
    }
};

static UINT8 char_to_tile_type(char c)
{
    switch (c)
    {
        case '.': return T_NORMAL;
        case 'F': return T_FIRE;
        case 'S': return T_SLIP;
        case 'G': return T_SAFE;
        default:  return T_VOID;
    }
}
