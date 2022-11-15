#pragma once

#include "activity.h"
#include "ctx.h"

typedef struct program_t program_t;

program_t* program_ctor(activity_t* activity, ctx_t* ctx);

void program_on_init(program_t* program);

void program_on_resize(program_t* program, int rows, int cols);

void program_on_keypress(program_t* program, int key);

