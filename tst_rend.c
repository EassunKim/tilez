#include <stdio.h>
#include <osbind.h>
#include "model.h"
#include "events.h"
#include "renderer.h"
#include "raster.h"

static int passed = 0;
static int failed = 0;

/*----- Function: show -----
* PURPOSE: draws one frame of the model with a label in the top-left corner,
*          waits for a key, then clears the screen
*
* INPUT:   label - what just happened (e.g. "W")
*          model - the model to draw
*          base  - the frame buffer
*
* OUTPUT:  None
*/
static void show(const char *label, const Model *model, UINT8 *base) {
    render(model, base);
    plot_string(base, 8, 8, (char *)label);
    Cnecin();
    clear_screen((UINT32 *)base);
}

/*----- Function: check -----
* PURPOSE: compares the model to the expected values and counts a pass or fail
*
* INPUT:   model    - the model to check
*          px,py,pf - expected player x, y and facing
*          ax,ay,af - expected ally x, y and facing
*          count    - expected move count
*          complete - expected level_complete
*
* OUTPUT:  None
*/
static void check(const Model *model,
                  UINT8 px, UINT8 py, UINT8 pf,
                  UINT8 ax, UINT8 ay, UINT8 af,
                  UINT16 count, BOOL complete) {
    if (model->player.x == px && model->player.y == py &&
        model->player.facing == pf &&
        model->ally.x == ax && model->ally.y == ay &&
        model->ally.facing == af &&
        model->move_count == count && model->level_complete == complete)
        passed++;
    else
        failed++;
}

/*----- Function: reset -----
* PURPOSE: reloads level 0 so a new test case starts from the spawns,
*          then draws it
*/
static void reset(Model *model, UINT8 *base) {
    model_load_level(model, 0);
    show("reset", model, base);
}

/*----- Function: test_spawn_move_rotate -----
* PURPOSE: same route as tst_modl.c: turn around, move, rotate, and walk
*          the player up next to the fire tile at (4,2)
*/
static void test(Model *model, UINT8 *base) {
    /* starting position*/
    reset(model, base);
    check(model, 2,3,NORTH, 7,3,SOUTH, 0, FALSE);

    /*--------MOVE TEST ------------*/
    asynch_turn_around(model);
    show("S", model, base);
    check(model, 2,3,SOUTH, 7,3,NORTH, 0, FALSE);

    asynch_rotate_left(model);
    show("A", model, base);
    check(model, 2,3,EAST, 7,3,WEST, 0, FALSE);

    asynch_forward(model);
    show("W", model, base);
    check(model, 3,3,EAST, 6,3,WEST, 2, FALSE);

    /*--------FIRE TEST ------------*/
    /* warp to be infront of fire tile*/
    player_init(&model->player, 3, 2, EAST);
    show("warp", model, base);
    check(model, 3,2,EAST, 6,3,WEST, 1, FALSE);

    asynch_forward(model);
    show("into fire", model, base);
    check(model, 2,3,NORTH, 7,3,SOUTH, 2, FALSE); /*expected to return to spawn with an additional move*/

    /*--------GOAL TEST ------------*/
    /*warp ally next to goal*/
    player_init(&model->ally, 6, 5, WEST);
    show("warp", model, base);
    check(model, 3,2,EAST, 6,3,WEST, 1, FALSE);

    asynch_forward(model);
    show("into fire", model, base);
    check(model, 2,3, NORTH, 5,5,WEST , 2, TRUE); /*expected level complete flag and player to have moved forward 1*/

}

/*----- Function: main -----
* PURPOSE: gets the screen address, hides the text cursor, runs the render
*          tests, then prints the pass/fail totals
*/
int main(void) {
    Model model;
    UINT8 *base = (UINT8 *)Physbase();

    printf("\033f");
    clear_screen((UINT32 *)base);

    test(&model, base);

    printf("\033e");
    printf("%d passed, %d failed\n", passed, failed);
    printf("press any key to exit\n");
    Cnecin();
    return 0;
}
