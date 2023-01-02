#include <stdlib.h>
#include <stdio.h>
#include <curses.h>

#include "activity.h"
#include "colors.h"

#include "scripture.h"

activity_t* activity_selectbook_ctor(activity_t* prev, ctx_t* ctx) {
  activity_t* super = activity_super_ctor(
    ACTIVITY_TYPE_SELECTBOOK, NULL, prev, ctx);

  return super;
}

void activity_selectbook_on_init(activity_t* activity)
{
  int rows = ctx_get_rows(activity->ctx);
  int cols = ctx_get_cols(activity->ctx);
  virtual_activity_on_resize(activity, rows, cols);
}

void activity_selectbook_on_resize(activity_t* activity, int rows, int cols) {
  char msg[80];
  sprintf(msg, "ACT Select book: rows = %d; cols = %d\n\r", rows, cols);
  addstr(msg);
	refresh();
}

void activity_selectbook_on_keypress(activity_t* activity, int key) {
  wattron(stdscr, COLOR_PAIR(MY_PAIR_DONE));
  char msg[80];
  sprintf(msg, "select-book key = %d\n\r", key);
  addstr(msg);
  wattron(stdscr, COLOR_PAIR(MY_PAIR_DONE));

  if (key == 27) {
    sprintf(msg, "Exiting book selection %d", key);
    _activity_discharge(activity);

    const bookinfo_t* allbooks = get_all_books();

    wattron(stdscr, COLOR_PAIR(MY_PAIR_MENU_SELECTED_HI));
    int i = 0;
    int n = NUMBER_OF_BOOKS;
    for (i = 0; i < n; i++) {
      const bookinfo_t book = allbooks[i];
      char msg[20];
      sprintf(msg, "%d - %s", i+1, book.title);
      addstr(msg);
    }
    wattroff(stdscr, COLOR_PAIR(MY_PAIR_MENU_SELECTED_HI));
  }
	refresh();
}
