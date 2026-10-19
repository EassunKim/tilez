#include "asynch.h"
#include "cond.h"

/*----- Function: asynch_forward -----
 PURPOSE: Handles the forward key (W). Moves the player then the ally, then
          applies any tile effects (slip, fire, goal).

 INPUT: model - the model to update

 OUTPUT: None
*/
void asynch_forward(Model *model) {
    if (model->level_complete)
        return;

    model_move_forward(model);
    cond_after_move(model);
}

/*----- Function: asynch_rotate_left -----
 PURPOSE: Handles the rotate-left key (A). Rotates both characters left.

 INPUT: model - the model to update

 OUTPUT: None
*/
void asynch_rotate_left(Model *model) {
    if (model->level_complete)
        return;

    model_rotate_left(model);
}

/*----- Function: asynch_rotate_right -----
 PURPOSE: Handles the rotate-right key (D). Rotates both characters right.

 INPUT: model - the model to update

 OUTPUT: None

*/
void asynch_rotate_right(Model *model) {
    if (model->level_complete)
        return;

    model_rotate_right(model);
}

/*----- Function: asynch_turn_around -----
 PURPOSE: Handles the turn-around key (S). Turns both characters 180 degrees.

 INPUT: model - the model to update

 OUTPUT: None
*/
void asynch_turn_around(Model *model) {
    if (model->level_complete)
        return;

    model_turn_around(model);
}
