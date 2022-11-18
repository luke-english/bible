#include <stdlib.h>
#include <stdio.h>

#include "activity.h"
#include "zcurses.h"
#include "scripture.h"
#include "colors.h"

typedef struct activity_t activity_t;

struct activity_t {
    ctx_t* ctx;
    activity_t* next;
    activity_t* prev;
    int type;
    int phase;
};

activity_t* activity_welcome_ctor(activity_t* prev, ctx_t* ctx) {
  activity_t* activity = malloc(sizeof(activity_t));

  activity->ctx = ctx;
  activity->next = NULL;
  activity->prev = prev;
  activity->type = ACTIVITY_TYPE_WELCOME;
  activity->phase = ACTIVITY_PHASE_ACTIVE;

  if (prev != NULL) {
    prev->next = activity;
  }

  return activity;
}

activity_t* activity_selectbook_ctor(activity_t* prev, ctx_t* ctx) {
  activity_t* activity = malloc(sizeof(activity_t));

  activity->ctx = ctx;
  activity->next = NULL;
  activity->prev = prev;
  activity->type = ACTIVITY_TYPE_SELECTBOOK;
  activity->phase = ACTIVITY_PHASE_ACTIVE;

  if (prev != NULL) {
    prev->next = activity;
  }

  return activity;
}

void virtual_activity_dtor(activity_t* activity) {
  if (activity->prev != NULL) {
    activity->prev->next = NULL;
  }
  free(activity);
}

activity_t* activity_get_next(activity_t* activity) {
  return activity->next;
}

activity_t* activity_get_prev(activity_t* activity) {
  return activity->prev;
}

int activity_get_phase(activity_t* activity) {
  return activity->phase;
}

void activity_activate(activity_t* activity) {
  activity->phase = ACTIVITY_PHASE_ACTIVE;
}

void activity_welcome_on_resize(activity_t* activity, int rows, int cols) {
  char msg[80];
  sprintf(msg, "RESIX %dx%d\n\r", rows, cols);
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
    activity->next = activity_selectbook_ctor(activity, activity->ctx);
    activity->phase = ACTIVITY_PHASE_BACKGROUND;
  }

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
    activity->phase = ACTIVITY_PHASE_COMPLETE;


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

void virtual_activity_on_resize(activity_t* activity, int rows, int cols) {
  char ch[80];
  switch (activity->type) {
    case ACTIVITY_TYPE_WELCOME:
      activity_welcome_on_resize(activity, rows, cols);
      break;
    case ACTIVITY_TYPE_SELECTBOOK:
      activity_selectbook_on_resize(activity, rows, cols);
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
    case ACTIVITY_TYPE_SELECTBOOK:
      activity_selectbook_on_keypress(activity, key);
      break;
    default:
      sprintf(ch, "Keypress not implemented for %d", activity->type);
      addstr(ch);
      break;
  }
}

