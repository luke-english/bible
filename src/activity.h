#pragma once

#include "ctx.h"

#define ACTIVITY_TYPE_WELCOME 1
#define ACTIVITY_TYPE_SELECTBOOK 2

typedef struct activity_t activity_t;

activity_t* activity_welcome_ctor(ctx_t* ctx);

void activity_welcome_dtor(activity_t* activity);

void virtual_activity_on_resize(activity_t* activity, int rows, int cols);

void virtual_activity_on_keypress(activity_t* activity, int key);

