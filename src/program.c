#include <stdlib.h>

#include "program.h"
#include "activity.h"
typedef struct program_t program_t;

struct program_t {
    activity_t* head_activity;
    activity_t* tail_activity;
    ctx_t* ctx;
};

program_t* program_ctor(activity_t* activity, ctx_t* ctx) {
  program_t* program = malloc(sizeof(program_t));
  program->ctx = ctx;
  program->head_activity = activity;
  program->tail_activity = activity;
  
  return program;
}

void program_on_resize(program_t* program, int rows, int cols) {
  virtual_activity_on_resize(program->head_activity, rows, cols);
}

void program_on_keypress(program_t* program, int key) {
  virtual_activity_on_keypress(program->head_activity, key);
}
