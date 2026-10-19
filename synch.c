#include "synch.h"

/*----- Function: synch_tick -----
 PURPOSE: Handles one clock tick (70 per second).

 INPUT: model - the model to update

 OUTPUT: None

 NOTE: this function is our synchronous event, but at the moment i don't believe anything happens on a time.
 so for now it is void.
*/
void synch_tick(Model *model) {
    (void)model;
}
