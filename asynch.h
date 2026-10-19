#ifndef ASYNCH_H
#define ASYNCH_H

#include "model.h"

/* Declarations */
void asynch_forward(Model *model);
void asynch_rotate_left(Model *model);
void asynch_rotate_right(Model *model);
void asynch_turn_around(Model *model);

#endif