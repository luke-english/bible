#include <stdlib.h>
#include <stdio.h>

#include "activity.h"
#include "feed.h"

#include "zcurses.h"
#include "colors.h"

typedef struct activity_evil_t activity_evil_t;

struct activity_evil_t
{
  activity_t *super;
  WINDOW *redwin;
};

WINDOW *_evil_create_newwin(int height, int width, int starty, int startx);

activity_t *activity_evil_ctor(activity_t *prev, ctx_t *ctx)
{
  activity_evil_t *self = malloc(sizeof(activity_evil_t));

  activity_t *super = activity_super_ctor(ACTIVITY_TYPE_EVIL, self, prev, ctx);

  return super;
}

void activity_evil_dtor(activity_evil_t *self)
{
  delwin(self->redwin);
  free(self);
}

int startx, starty, width, height;

extern WINDOW* ACTWIN;


void activity_evil_on_init(activity_t *activity)
{
  curs_set(1);

  int rows = ctx_get_rows(activity->ctx);
  int cols = ctx_get_cols(activity->ctx);
  virtual_activity_on_resize(activity, rows, cols);
}

void activity_evil_on_resize(activity_t *activity, int rows, int cols)
{
  activity_evil_t *self = _activity_get_details(activity);
  WINDOW *win = self->redwin;

  height = LINES - (2*2 +1);
  width = COLS - (2*4);
  starty = (LINES - height) / 2; /* Calculating for a center placement */
  startx = (COLS - width) / 2;   /* of the window		*/

  resize_term(LINES, COLS); /* Only required for WASM, */
                            /* NCurses does this automatically */

  touchwin(stdscr);
  bkgd(COLOR_PAIR(MY_PAIR_DESKTOP));
  refresh();

  self->redwin = _evil_create_newwin(height, width, starty, startx);
  win = self->redwin;
  ACTWIN = win;

  // wattron(win, COLOR_PAIR(MY_PAIR_HINT));
  char* msg = malloc(bblob_size + 2); // +2 for "\n\r"

  feed_t* feed = feed_ctor(bblob, bblob_size);

  lines_t *lines = lines_ctor();
  size_t lines_coun = feed_get_visible_lines(feed, height, width, lines);
  int y;
  
  uint8_t* str;
  for (y = 0; (str = lines_take(lines)) != NULL; y++) {


  sprintf(msg, "%d/%d. %s\n\r", y+1, height, str);
     mvwaddstr(win, y, 0, msg);
  }
  wrefresh(win);
}

void activity_evil_on_keypress(activity_t *activity, int key)
{
  activity_evil_t *self = _activity_get_details(activity);
  WINDOW *win = self->redwin;

  wrefresh(win);

  wattron(win, COLOR_PAIR(MY_PAIR_DONE));
  char* msg = malloc(5); // +2 for "\n\r"
  sprintf(msg, "%c", key);
  waddstr(win, msg);
  free(msg);
  wattroff(win, COLOR_PAIR(MY_PAIR_DONE));

  wrefresh(win);

  if (key == 27)
  {
    _activity_discharge(activity);
    curs_set(1);

    touchwin(stdscr);
    refresh();
  }
}

WINDOW *_evil_create_newwin(int height, int width, int starty, int startx)
{
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  wbkgd(local_win, COLOR_PAIR(MY_PAIR_HINT));

  return local_win;
}
