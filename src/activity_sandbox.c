#include <stdlib.h>
#include <stdio.h>

#include "activity.h"

#include "zcurses.h"
#include "colors.h"

typedef struct activity_sandbox_t activity_sandbox_t;

struct activity_sandbox_t {
    activity_t* super;
    WINDOW* redwin;
};

activity_t* activity_sandbox_ctor(activity_t* prev, ctx_t* ctx) {
  activity_sandbox_t* self = malloc(sizeof(activity_sandbox_t));

  activity_t* super = activity_super_ctor(
    ACTIVITY_TYPE_SANDBOX, self, prev, ctx);

  return super;
}

void activity_sandbox_dtor(activity_sandbox_t* self) {
  delwin(self->redwin);
  free(self);
}

void activity_sandbox_on_resize(activity_t* activity, int rows, int cols) {
  activity_sandbox_t* self = _activity_get_details(activity);
  char msg[80];
  sprintf(msg, "RESIX %dx%d\n\r", rows, cols);
  bkgdset(COLOR_PAIR(MY_PAIR_DESKTOP));
  bkgd(COLOR_PAIR(MY_PAIR_DESKTOP));

  refresh();

  WINDOW* win = self->redwin;
  delwin(win);

  win = newwin(15, 37, 2, 10);
  self->redwin = win;

  wbkgdset(win, COLOR_PAIR(MY_PAIR_ALERT));

  wbkgd(win, COLOR_PAIR(MY_PAIR_ALERT));
  wrefresh(win);
  // box(win, 0, 0);

  mvwprintw(win, 0, 1, "Greeter\n\r");
  mvwprintw(win, 1, 1, "Hello\n\r");
  waddstr(win, msg);

  // refreshing the window
  wrefresh(win);
}

void activity_sandbox_on_keypress(activity_t* activity, int key) {
  activity_sandbox_t* self = _activity_get_details(activity);
  WINDOW* win = self->redwin;
  wattron(win, COLOR_PAIR(MY_PAIR_LABEL));
  char msg[80];
  sprintf(msg, "sb %d,\n\r", key);
  waddstr(win, msg);

	wrefresh(win);

  if (key == 27) {
    sprintf(msg, "Exiting book sandbox %d", key);
    _activity_discharge(activity);
    wbkgd(stdscr, COLOR_PAIR(MY_PAIR_LABEL));
  }
}
