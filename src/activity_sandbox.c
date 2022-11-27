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

	height = 4;
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

  // print pressed key
  wattron(win, COLOR_PAIR(MY_PAIR_LABEL));
  char msg[80];
  sprintf(msg, "sb_%d,\n\r", key);
  waddstr(win, msg);
  wattroff(win, COLOR_PAIR(MY_PAIR_LABEL));

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
  char msg[80];
  char aleph[5] = "א";
  char ltu[4] = "ė";
  char burger[45] = "🍔";
  char unicorn[45] = "🦄";
  mvwaddstr(local_win, 1, 2, "  burger");
  mvwaddstr(local_win, 2, 2, "  unicorn");

  // This refresh is needed if we want to write multibyte characters without
  // distorting surrounding code
  wrefresh(local_win);

  // After refresh all this will be applied on top
  mvwaddstr(local_win, 1, 1, burger);
  mvwaddstr(local_win, 2, 1, unicorn);
 
  wattroff(local_win, COLOR_PAIR(MY_PAIR_LABEL));

	wrefresh(local_win);

	return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	wrefresh(local_win);
	delwin(local_win);
}