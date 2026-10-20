#include <stdio.h>
#include "model.h"
#include "events.h"

static int passed = 0;
static int failed = 0;

static char dir_char(UINT8 facing) {
    switch (facing) {
        case NORTH: return 'N';
        case EAST:  return 'E';
        case SOUTH: return 'S';
        case WEST:  return 'W';
        default:    return '?';
    }
}

static void print_model(const char *label, const Model *model) {
    printf("%-6s P(%d,%d,%c) A(%d,%d,%c) c=%u win=%d\n", label,
           model->player.x, model->player.y, dir_char(model->player.facing),
           model->ally.x, model->ally.y, dir_char(model->ally.facing),
           model->move_count, model->level_complete);
    printf("       spawns P(%d,%d,%c) A(%d,%d,%c) level=%d\n",
           model->player_spawn.x, model->player_spawn.y,
           dir_char(model->player_spawn.facing),
           model->ally_spawn.x, model->ally_spawn.y,
           dir_char(model->ally_spawn.facing), model->level_select);
}

/*----- Function: check -----
* PURPOSE: compares the model to the expected values and prints PASS or FAIL.
*          On a FAIL it also prints the expected values. Updates passed / failed.
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
        model->move_count == count && model->level_complete == complete) {
        passed++;
        printf("       PASS\n");
    } else {
        failed++;
        printf("       FAIL expected P(%d,%d,%c) A(%d,%d,%c) c=%u win=%d\n",
               px, py, dir_char(pf), ax, ay, dir_char(af), count, complete);
    }
}

/*----- Function: reset-----
* PURPOSE: Resets the model so I can move onto another test case
*/
static void reset(Model *model) {
    model_load_level(model, 0);
    print_model("reset", model);
}

/*----- Function: wait_key -----
* PURPOSE: waits for a key input so all tests can be shown in turn
*/
static void wait_key(void) {
    printf("-- press Return --\n");
    getchar();
}

static void test_spawn_move_rotate(Model *model) {
    printf("\n=== spawn, move, rotate, walk to fire ===\n");

    reset(model);
    check(model, 2,3,NORTH, 7,3,SOUTH, 0, FALSE);

    asynch_turn_around(model);
    print_model("S", model);
    check(model, 2,3,SOUTH, 7,3,NORTH, 0, FALSE);

    asynch_forward(model);
    print_model("W", model);
    check(model, 2,4,SOUTH, 7,2,NORTH, 1, FALSE);

    asynch_rotate_left(model);
    print_model("A", model);
    check(model, 2,4,EAST, 7,2,WEST, 1, FALSE);

    asynch_forward(model);
    print_model("W", model);
    check(model, 3,4,EAST, 6,2,WEST, 2, FALSE);

    asynch_rotate_left(model);
    print_model("A", model);
    check(model, 3,4,NORTH, 6,2,SOUTH, 2, FALSE);

    asynch_forward(model);
    print_model("W", model);
    check(model, 3,3,NORTH, 6,3,SOUTH, 3, FALSE);

    asynch_forward(model);
    print_model("W", model);
    check(model, 3,2,NORTH, 6,4,SOUTH, 4, FALSE);

    wait_key();
}

int main(void) {
    Model model;

    test_spawn_move_rotate(&model);

    printf("\n%d passed, %d failed\n", passed, failed);
    wait_key();
    return 0;
}
