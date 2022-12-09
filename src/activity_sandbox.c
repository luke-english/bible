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

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

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

int startx, starty, width, height;

void activity_sandbox_on_resize(activity_t* activity, int rows, int cols) {
  activity_sandbox_t* self = _activity_get_details(activity);
  WINDOW* win = self->redwin;

	height = 10;
	width = 20;
	starty = (LINES - height) / 2;	/* Calculating for a center placement */
	startx = (COLS - width) / 2;	/* of the window		*/
  
  bkgd(COLOR_PAIR(MY_PAIR_DESKTOP));
  refresh();
  curs_set(0);
	self->redwin = create_newwin(height, width, starty, startx);
}

void activity_sandbox_on_keypress(activity_t* activity, int key) {
  activity_sandbox_t* self = _activity_get_details(activity);
  WINDOW* win = self->redwin;

	wrefresh(win);

  if (key == 'h') {
    destroy_win(win);

    touchwin(stdscr);
    bkgd(COLOR_PAIR(MY_PAIR_DESKTOP));
    refresh();

    startx--;
    self->redwin = create_newwin(height, width, starty, startx);
  }
  if (key == 'k') {
    destroy_win(win);

    touchwin(stdscr);
    bkgd(COLOR_PAIR(MY_PAIR_DESKTOP));
    refresh();

    starty--;
    self->redwin = create_newwin(height, width, starty, startx);
  }
  if (key == 'j') {
    destroy_win(win);

    touchwin(stdscr);
    bkgd(COLOR_PAIR(MY_PAIR_DESKTOP));
    refresh();

    starty++;
    self->redwin = create_newwin(height, width, starty, startx);
  }
  if (key == 'l') {
    destroy_win(win);

    touchwin(stdscr);
    bkgd(COLOR_PAIR(MY_PAIR_DESKTOP));
    refresh();

    startx++;
    self->redwin = create_newwin(height, width, starty, startx);
  }
  if (key == 27) {
    _activity_discharge(activity);
    curs_set(1);

    touchwin(stdscr);
    refresh();
  }
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
  WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
  wbkgd(local_win, COLOR_PAIR(MY_PAIR_ALERT));

	// box(local_win, 0 , 0);  
  wborder(local_win, '|', '|', '-', '-', '+', '+', '+', '+');

  wattron(local_win, COLOR_PAIR(MY_PAIR_LABEL));

  // Emojis are tricky still it is unclear what is the width of some
  // Printing them separately, and leavinig some trailing space
  // might help.
  mvwaddstr(local_win, 1, 2, "🍔  "); mvwaddstr(local_win, 1, 4, "hamburger");
  mvwaddstr(local_win, 2, 2, "⚡  "); mvwaddstr(local_win, 2, 4, "flash");
  mvwaddstr(local_win, 3, 2, "ė  ");  mvwaddstr(local_win, 3, 4, "lithuanian");
  mvwaddstr(local_win, 4, 2, "א  ");  mvwaddstr(local_win, 4, 4, "hebrew");
  mvwaddstr(local_win, 5, 2, "🥸  "); mvwaddstr(local_win, 5, 4, "ugly face");
  mvwaddstr(local_win, 6, 2, "🍀  "); mvwaddstr(local_win, 6, 4, "clover");
  mvwaddstr(local_win, 7, 2, "🐠 "); mvwaddstr(local_win, 7, 4, "fish");
  mvwaddstr(local_win, 8, 2, "🦄  "); mvwaddstr(local_win, 8, 4, "unicorn");
	wrefresh(local_win);

  wattroff(local_win, COLOR_PAIR(MY_PAIR_LABEL));
	return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	wrefresh(local_win);
	delwin(local_win);
}