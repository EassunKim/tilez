/* tst_modl.c - simple test driver for the TileZ model.
 *
 * Loads level 0, sends a few inputs, and prints the model after each one.
 */

#include <stdio.h>
#include "model.h"
#include "events.h"

/*----- Function: print_model -----
 PURPOSE: Prints the player, ally, move count and win flag

 INPUT:  label - what just happened
         model - the model to print
*/
static void print_model(const char *label, const Model *model) {
    printf("%-10s player (%d,%d) f=%d  ally (%d,%d) f=%d  moves=%u  win=%d\n",
           label,
           model->player.x, model->player.y, model->player.facing,
           model->ally.x, model->ally.y, model->ally.facing,
           model->move_count, model->level_complete);
}

int main(void) {
    Model model;

    model_load_level(&model, 0);
    print_model("load", &model);

    asynch_forward(&model);
    print_model("W", &model);

    asynch_rotate_left(&model);
    print_model("A", &model);

    asynch_forward(&model);
    print_model("W", &model);

    asynch_turn_around(&model);
    print_model("S", &model);

    asynch_forward(&model);
    print_model("W", &model);

    return 0;
}
