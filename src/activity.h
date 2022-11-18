#pragma once

#include "ctx.h"

#define ACTIVITY_TYPE_WELCOME    1
#define ACTIVITY_TYPE_SELECTBOOK 2
#define ACTIVITY_TYPE_SANDBOX    3

#define ACTIVITY_PHASE_BACKGROUND 1
#define ACTIVITY_PHASE_ACTIVE     2
#define ACTIVITY_PHASE_COMPLETE   3

typedef struct activity_t activity_t;

activity_t* activity_welcome_ctor(activity_t* previous, ctx_t* ctx);

activity_t* activity_selectbook_ctor(activity_t* previous, ctx_t* ctx);

activity_t* activity_sandbox_ctor(activity_t* previous, ctx_t* ctx);

void virtual_activity_dtor(activity_t* activity);

activity_t* activity_get_next(activity_t* activity);

activity_t* activity_get_prev(activity_t* activity);

int activity_get_phase(activity_t* activity);

void activity_activate(activity_t* activity);

void virtual_activity_on_resize(activity_t* activity, int rows, int cols);

void virtual_activity_on_keypress(activity_t* activity, int key);

