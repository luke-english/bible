#include <stdlib.h>
#include <stdio.h>

#include "zcurses.h"
#include "activity.h"

typedef struct activity_t activity_t;

struct activity_t {
    ctx_t* ctx;
    activity_t* next;
    activity_t* prev;
    int type;
};

activity_t* activity_welcome_ctor(ctx_t* ctx) {
  activity_t* activity = malloc(sizeof(activity_t));
  activity->type = ACTIVITY_TYPE_WELCOME;
  activity->ctx = ctx;
  return activity;
}

void activity_welcome_on_resize(activity_t* activity, int rows, int cols) {
  char msg[80];
  sprintf(msg, "Value of Pi rows = %d; cols = %d", rows, cols);
  addstr(msg);
}

void activity_welcome_on_keypress(activity_t* activity, int key) {
  char msg[80];
  sprintf(msg, "Key pressed in welcome = %d", key);
  addstr(msg);
}

void virtual_activity_on_resize(activity_t* activity, int rows, int cols) {
  char ch[80];
  switch (activity->type) {
    case ACTIVITY_TYPE_WELCOME:
      activity_welcome_on_resize(activity, rows, cols);
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
    case ACTIVITY_TYPE_WELCOME:
      activity_welcome_on_keypress(activity, key);
      break;
    default:
      sprintf(ch, "Keypress not implemented for %d", activity->type);
      addstr(ch);
      break;
  }
}

