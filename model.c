#include <stddef.h>
#include "model.h"

/*----- Function: model_reset_level -----
 PURPOSE: Puts the player and ally back on their spawns. Used after a fire tile.

 INPUT: model - the model to reset

 OUTPUT: None
*/
void model_reset_level(Model *model) {
    model->player = model->player_spawn;
    model->ally = model->ally_spawn;

}
/*----- Function: model_load_level -----
 PURPOSE: Loads a level into the model: grid, spawns, counters, positions.

 INPUT: model - the model to load into
        select - which level to load

 OUTPUT: None
*/
void model_load_level(Model *model, UINT8 select) {
    const Level *lvl;

    /*load level*/
    lvl = level_get(select);
    level_build_grid(lvl, model->grid);

    /*set spawns from level layout*/
    model->player_spawn = lvl->player_spawn;
    model->ally_spawn = lvl->ally_spawn;

    /*set trackers*/
    model->move_count = 0;
    model->level_complete = FALSE;
    model->level_select = select;
    
    model_reset_level(model);
    return;

}

/* ------------------ Checks ------------------------- */

/*----- Function: model_tile_at -----
 PURPOSE: Returns the type of the tile at a cell.

 INPUT: model - the model to read
        x, y  - the cell (may be off-grid)

 OUTPUT: the tile's type, or T_VOID if the cell is off the grid
*/
UINT8 model_tile_at(const Model *model, INT16 x, INT16 y) {
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) {
        return T_VOID;
    }

    return model->grid[y][x].type;
}

/*----- Function: model_can_enter -----
 PURPOSE: Checks whether a character can step one cell in the direction it faces.

 INPUT: model - the model to read
        character - the character trying to move
        other - the other character (blocks the cell it stands on)

 OUTPUT: TRUE if the move is allowed, FALSE if blocked by:
         - void / off-grid target
         - a wall on the mover's tile, facing side
         - a wall on the target tile, opposite side
         - the other character standing on the target

 NOTE:  in this function, opposite is used to get the wall that would be facing the player.
        ex. player moving north has to worry about the north of their own tile and the south wall
        of the next tile.
*/
BOOL model_can_enter(const Model *model, const Player *character, const Player *other) {
    INT16 next_x;
    INT16 next_y;
    UINT8 opposite; /*the opposite direction of the way character is currently facing*/

    player_forward_cell(character, &next_x, &next_y);
    opposite = (character->facing +2) % 4;

    /*tile checks
    * don't allow movement if void, wall, 
    */
   /*check if void*/
    if (model_tile_at(model, next_x, next_y) == T_VOID)
        return FALSE;

    /*check wall*/
    if (tile_check_walls(&model->grid[character->y][character->x], character->facing))
        return FALSE;

    /* check opposite wall*/
    if (tile_check_walls(&model->grid[next_y][next_x], opposite))
        return FALSE;

    /*check if other character is blocking movement*/
    if (other->x == next_x && other->y == next_y)
        return FALSE;

    return TRUE;
}

/* ------------------------- movement ---------------------------- */

/*----- Function: try_step -----
 PURPOSE: Moves a character one cell forward if model_can_enter allows it.

 INPUT: model - the model the character is in
        mover - the character to move
        other - the other character

 OUTPUT: TRUE if the character moved, FALSE if it was blocked
*/
static BOOL try_step(Model *model, Player *mover, const Player *other) {
    INT16 next_x;
    INT16 next_y;

    if (!model_can_enter(model, mover, other)) {
        return FALSE;
    }
    player_forward_cell(mover, &next_x, &next_y);
    player_set_position(mover, next_x, next_y);
    return TRUE;
}

/*----- Function: model_move_forward -----
 PURPOSE: Handles a forward input: player steps, then ally steps.

 INPUT: model - the model to update

 OUTPUT: None

 ASSUMPTIONS: The player resolves first; the ally is checked against the
              player's NEW position. move_count goes up only if at least
              one character moved.
*/
void model_move_forward(Model *model) {
    BOOL player_moved;
    BOOL ally_moved;

    /*move both characters*/
    player_moved = try_step(model, &model->player, &model-> ally);
    ally_moved = try_step(model, &model->ally, &model->player);

    if (player_moved || ally_moved) {
        model->move_count++;
    }
}

/*----- Function: model_rotate_left -----
 PURPOSE: Rotates both characters left. Does not count as a move.

 INPUT: model - the model to update

 OUTPUT: None
*/
void model_rotate_left(Model *model) {
    player_rotate_left(&model->player);
    player_rotate_left(&model->ally);
}

/*----- Function: model_rotate_right -----
 PURPOSE: Rotates both characters right. Does not count as a move.

 INPUT: model - the model to update

 OUTPUT: None
*/
void model_rotate_right(Model *model) {
    player_rotate_right(&model->player);
    player_rotate_right(&model->ally);
}

/*----- Function: model_turn_around -----
* PURPOSE: Turns both characters 180 degrees. Does not count as a move.
*
* INPUT: model - the model to update
*
* OUTPUT: None
*/
void model_turn_around(Model *model) {
    player_turn_around(&model->player);
    player_turn_around(&model->ally);
}

/*----- Function: model_slip -----
* PURPOSE: Slides a character one extra cell after landing on a slip tile.
*          Called from cond.c.
*
* INPUT: model - the model the character is in
*        character   - the character on the slip tile
*        other - the other character
*
* OUTPUT: TRUE if the character slid, FALSE if blocked (it stays on the slip tile)
*
* Note:   When slip is called it will be after a player has moved to a tile, so this 
*         is just another step after the fact, if not blocked.
*/
BOOL model_slip(Model *model, Player *character, const Player *other) {
   return try_step(model, character, other);
}
