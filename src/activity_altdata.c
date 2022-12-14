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


void activity_altdata_on_init(activity_t* activity) {


  if (bblob_size > 0) {
    _activity_create_next_evil(activity);    
  } else {
    _activity_create_next_welcome(activity);
  }
}

void activity_altdata_on_resize(activity_t* activity, int rows, int cols) {
}

void activity_altdata_on_keypress(activity_t* activity, int key) {

  wattron(stdscr, COLOR_PAIR(MY_PAIR_WRONG));
  
  if (bblob_size > 0) { 
    char* msg = malloc(bblob_size + 2); // +2 for "\n\r"
    sprintf(msg, "%s\n\r", bblob);
    addstr(msg);
    free(msg);
    refresh();
  } else {
    addstr("Press ESC to continue...\n\r");
  }

  wattroff(stdscr, COLOR_PAIR(MY_PAIR_WRONG));
	refresh();

  if (key == 27) {
    _activity_create_next_welcome(activity);
  }
}
