#include "cond.h"

/*----- Function: apply_slip -----
 PURPOSE: If a character is on a slip tile, slides it one extra cell.

 INPUT: model - the model the character is in
        who   - the character to check
        other - the other character (can block the slide)

 OUTPUT: None

 NOTE:   SLIP function not yet implemented
*/
static void apply_slip(Model *model, Player *who, const Player *other) {
    if (model_tile_at(model, who->x, who->y) == T_SLIP)
        model_slip(model, who, other);
}

/*----- Function: check_fire -----
 PURPOSE: If either character is on a fire tile, resets both to their spawns.

 INPUT: model - the model to check

 OUTPUT: TRUE if a reset happened, FALSE otherwise
*/
static BOOL check_fire(Model *model) {
    if (model_tile_at(model, model->player.x, model->player.y) == T_FIRE ||
        model_tile_at(model, model->ally.x, model->ally.y) == T_FIRE) {
        model_reset_level(model);
        return TRUE;
    }
    return FALSE;
}

/*----- Function: check_goal -----
 PURPOSE: Marks the level complete if the ally is on the goal tile.

 INPUT: model - the model to check

 OUTPUT: None
*/
static void check_goal(Model *model) {
    if (model_tile_at(model, model->ally.x, model->ally.y) == T_GOAL)
        model->level_complete = TRUE;
}

/*----- Function: cond_after_move -----
 PURPOSE: Applies all tile effects after a forward move, in order:
          slips, then fire, then goal.

 INPUT: model - the model to update

 OUTPUT: None

 NOTE:   Slips are resolved first(on both entities) so a slide onto fire is caught.
*/
void cond_after_move(Model *model) {
    apply_slip(model, &model->player, &model->ally);
    apply_slip(model, &model->ally, &model->player);

    if (check_fire(model))
        return;

    check_goal(model);
}
