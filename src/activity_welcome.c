#include <stdlib.h>
#include <stdio.h>

#include "activity.h"

#include "zcurses.h"
#include "colors.h"

activity_t* activity_welcome_ctor(activity_t* prev, ctx_t* ctx) {
  activity_t* super = activity_super_ctor(
    ACTIVITY_TYPE_WELCOME, NULL, prev, ctx);

  return super;
}

void activity_welcome_on_resize(activity_t* activity, int rows, int cols) {
  char msg[80];
  sprintf(msg, "SZ: %dx%d!__\n\r", rows, cols);
  addstr(msg);
	refresh();
}

void activity_welcome_on_keypress(activity_t* activity, int key) {

  wattron(stdscr, COLOR_PAIR(MY_PAIR_WRONG));
  char msg[80];
  sprintf(msg, "%d,\n\r", key);
  addstr(msg);
  wattroff(stdscr, COLOR_PAIR(MY_PAIR_WRONG));

	refresh();

  if ((key == 10) || (key == 13)) {
    _activity_create_next_selectbook(activity);
  }
  if (key == 's') {
    _activity_create_next_sandbox(activity);
  }
}
