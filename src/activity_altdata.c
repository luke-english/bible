#include <stdlib.h>
#include <stdio.h>

#include "activity.h"

#include "zcurses.h"
#include "colors.h"

extern size_t bblob_size;
extern uint8_t* bblob;

activity_t* activity_altdata_ctor(activity_t* prev, ctx_t* ctx) {
  activity_t* super = activity_super_ctor(
    ACTIVITY_TYPE_ALTDATA, NULL, prev, ctx);

  return super;
}

void activity_altdata_on_resize(activity_t* activity, int rows, int cols) {
  char msg[80];
  sprintf(msg, "ALTDATA RESIZE: %dx%d!__\n\r", rows, cols);
  addstr(msg);
	refresh();
}

void activity_altdata_on_keypress(activity_t* activity, int key) {

  wattron(stdscr, COLOR_PAIR(MY_PAIR_WRONG));
  char msg[80];
  
  sprintf(msg, "altdata ESC to quit%d,\n\r", key);
  addstr(msg);
  
  sprintf(msg, "bblob size: %zu! bblob itself: %s\n\r", bblob_size, bblob);
  addstr(msg);

  wattroff(stdscr, COLOR_PAIR(MY_PAIR_WRONG));

	refresh();

  if (key == 27) {
    _activity_create_next_welcome(activity);
  }
}
