#include <stdlib.h>
#include <stdio.h>
#include <curses.h>

#include "activity.h"
#include "colors.h"

typedef struct activity_t activity_t;

struct activity_t {
    ctx_t* ctx;
    activity_t* next;
    activity_t* prev;
    int type;
    int phase;
    void* details;
};

activity_t* activity_super_ctor(int type, void* details, activity_t* prev, ctx_t* ctx) {
  activity_t* super = malloc(sizeof(activity_t));

  super->ctx = ctx;
  super->next = NULL;
  super->prev = prev;
  super->type = type;
  super->phase = ACTIVITY_PHASE_ACTIVE;
  super->details = details;

  if (prev != NULL) {
    prev->next = super;
  }

  return super;
}

void activity_activate(activity_t* activity) {
  activity->phase = ACTIVITY_PHASE_ACTIVE;
}

int activity_get_phase(activity_t* activity) {
  return activity->phase;
}

activity_t* activity_get_prev(activity_t* activity) {
  return activity->prev;
}

activity_t* activity_get_next(activity_t* activity) {
  return activity->next;
}

//
// Protected methods
//
void* _activity_get_details(activity_t* activity) {
  return activity->details;
}

void _activity_discharge(activity_t* activity) {
  activity->phase = ACTIVITY_PHASE_COMPLETE;
}

//
// All child implementations:
// 
// But:
// 1. declare regular c-tors for use from program, tests
//    and factory methods bellow
// 2. define protected factory methods _activity_create_next_* 
//
activity_t* activity_evil_ctor(activity_t* prev, ctx_t* ctx);
activity_t* activity_sandbox_ctor(activity_t* prev, ctx_t* ctx);
activity_t* activity_welcome_ctor(activity_t* prev, ctx_t* ctx);
activity_t* activity_selectbook_ctor(activity_t* prev, ctx_t* ctx);
activity_t* activity_sandbox_ctor(activity_t* prev, ctx_t* ctx);

void _activity_create_next_evil(activity_t* activity) {
  activity->next = activity_evil_ctor(activity, activity->ctx);
  activity->phase = ACTIVITY_PHASE_BACKGROUND;
}

void _activity_create_next_sandbox(activity_t* activity) {
  activity->next = activity_sandbox_ctor(activity, activity->ctx);
  activity->phase = ACTIVITY_PHASE_BACKGROUND;
}

void _activity_create_next_welcome(activity_t* activity) {
  activity->next = activity_welcome_ctor(activity, activity->ctx);
  activity->phase = ACTIVITY_PHASE_BACKGROUND;
}

void _activity_create_next_selectbook(activity_t* activity) {
  activity->next = activity_selectbook_ctor(activity, activity->ctx);
  activity->phase = ACTIVITY_PHASE_BACKGROUND;
}

#include "activity_altdata.c"
#include "activity_evil.c"
#include "activity_sandbox.c"
#include "activity_welcome.c"
#include "activity_selectbook.c"

//
// Abstract methods switching to appropriate implementations
//
void virtual_activity_dtor(activity_t* activity) {
  if (activity->prev != NULL) {
    activity->prev->next = NULL;
  }

  if (activity->type == ACTIVITY_TYPE_EVIL) {
    activity_evil_dtor(activity->details);
  }  
  if (activity->type == ACTIVITY_TYPE_SANDBOX) {
    activity_sandbox_dtor(activity->details);
  }
  
  free(activity);
}

void virtual_activity_on_init(activity_t* activity) {
  char ch[80];
  switch (activity->type) {
    case ACTIVITY_TYPE_ALTDATA:
      activity_altdata_on_init(activity);
      break;
    case ACTIVITY_TYPE_EVIL:
      activity_evil_on_init(activity);
      break;
    case ACTIVITY_TYPE_WELCOME:
      activity_welcome_on_init(activity);
      break;
    case ACTIVITY_TYPE_SELECTBOOK:
      activity_selectbook_on_init(activity);
      break;
    case ACTIVITY_TYPE_SANDBOX:
      activity_sandbox_on_init(activity);
      break;
    default:
      sprintf(ch, "Init not implemented for %d", activity->type);
      addstr(ch);
      break;
  }
}

void virtual_activity_on_resize(activity_t* activity, int rows, int cols) {
  char ch[80];
  switch (activity->type) {
    case ACTIVITY_TYPE_ALTDATA:
      activity_altdata_on_resize(activity, rows, cols);
      break;    
    case ACTIVITY_TYPE_EVIL:
      activity_evil_on_resize(activity, rows, cols);
      break;
    case ACTIVITY_TYPE_WELCOME:
      activity_welcome_on_resize(activity, rows, cols);
      break;
    case ACTIVITY_TYPE_SELECTBOOK:
      activity_selectbook_on_resize(activity, rows, cols);
      break;
    case ACTIVITY_TYPE_SANDBOX:
      activity_sandbox_on_resize(activity, rows, cols);
      break;
    default:
      sprintf(ch, "Resize not implemented for %d", activity->type);
      addstr(ch);
      break;
  }
}

void virtual_activity_on_keypress(activity_t* activity, int key) {
  char ch[80];
  switch (activity->type) {
    case ACTIVITY_TYPE_ALTDATA:
      activity_altdata_on_keypress(activity, key);
      break;
    case ACTIVITY_TYPE_EVIL:
      activity_evil_on_keypress(activity, key);
      break;
    case ACTIVITY_TYPE_WELCOME:
      activity_welcome_on_keypress(activity, key);
      break;
    case ACTIVITY_TYPE_SELECTBOOK:
      activity_selectbook_on_keypress(activity, key);
      break;
    case ACTIVITY_TYPE_SANDBOX:
      activity_sandbox_on_keypress(activity, key);
      break;
    default:
      sprintf(ch, "Keypress not implemented for %d", activity->type);
      addstr(ch);
      break;
  }
}
