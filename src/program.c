#include <stdlib.h>
#include <stdio.h>

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

void _program_follow_head_activity(program_t* program) {
  while (activity_get_phase(program->head_activity) == ACTIVITY_PHASE_BACKGROUND) {

    while (activity_get_next(program->head_activity) != NULL) {
      program->head_activity = activity_get_next(program->head_activity);
    }

    virtual_activity_on_init(program->head_activity);
  }

  while (activity_get_phase(program->head_activity) == ACTIVITY_PHASE_COMPLETE) {
    activity_t* prev = activity_get_prev(program->head_activity);
    virtual_activity_dtor(program->head_activity);
    program->head_activity = prev;

    activity_activate(program->head_activity);

    if (program->head_activity == NULL) {
      // TODO End program
    }

    virtual_activity_on_init(program->head_activity);
  }
}

void program_on_init(program_t* program) {  
  virtual_activity_on_init(program->head_activity);

  _program_follow_head_activity(program);
}

void program_on_resize(program_t* program, int rows, int cols) {
  virtual_activity_on_resize(program->head_activity, rows, cols);

  ctx_set_size(program->ctx, rows, cols);
}

void program_on_keypress(program_t* program, int key) {
  virtual_activity_on_keypress(program->head_activity, key);
  _program_follow_head_activity(program);
}
