# TileZ

COMP 2659, Mount Royal University, Fall 2026. A tile puzzle game for the Atari ST (640x400 monochrome).

## Authors

- Matthew Kim
- Mohamad Hamade
- Ujjwal Bhandari

## Checkpoint 2 - October 19

Model, raster graphics library, and renderer.

### Mohamad Hamade ()

| File | Description |
| --- | --- |

### Ujjwal Bhandari ()

| File | Description |
| --- | --- |

### Matthew Kim (model and events)

| File | Description |
| --- | --- |
| `types.h` | Types headers and declarations used for model, includes signed/unsigned int8/16/32 and a boolean type |
| `player.h` / `player.c` |Player behavior files. Used to outline the behaviors of both player and ally objects. NOTE: currently and for the sake of time ally and player are drawn in the same way, this will be fixed when animating them both in checkpoint3 |
| `tile.h` / `tile.c` | Tile behavior files. Includes the way in which tiles are differentiated and how to determine when walls are present |
| `levels.h` / `levels.c` | Level behavior files. Uses tile and player to outline our grid based levels, as well as how we will be storing level designs |
| `model.h` / `model.c` | Model files as outlined in checkpoint 2, uses all files above. Creates a model holding objects and their values as well as functions that will be used by events (move forward, turn, etc,)|
| `events.h` | Header file for next 3 anvent files. From d2l|
| `asynch.h` / `asynch.c` | Handler for asynchronous events like movement and rotation |
| `synch.h` / `synch.c` | Handler for synchronous events. As I understand it, we do not currently have any synchronous events reliant on the clock. We will include them as we animate but for not it is a file with a dead function in it. |
| `cond.h` / `cond.c` | Checks conditional events like intersecting a special tile |
| `tst_modl.c` | Tester for my model. Includes a check function that will essentially check the values in the model and return a pass or fail after one of our events is called.|
| `tst_rast.c` | Similar to the above tester. Print replaced with render() from renderer.c but still returns a pass or fail and the totals |
| `MAKEFILE` | Our make file. AI (Gemini) was prompted to teach me how to make a 3 way make file (since we have 3 testers) as I had never seen it. An example was provided but code is all my own |

