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
    void* details;
};

activity_t* activity_welcome_ctor(activity_t* prev, ctx_t* ctx);
activity_t* activity_selectbook_ctor(activity_t* prev, ctx_t* ctx);
activity_t* activity_sandbox_ctor(activity_t* prev, ctx_t* ctx);

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

// Sandbox ///// 

typedef struct activity_sandbox_t activity_sandbox_t;

struct activity_sandbox_t {
    WINDOW* redwin;
};

activity_t* activity_sandbox_ctor(activity_t* prev, ctx_t* ctx) {
  activity_t* activity = malloc(sizeof(activity_t));

  activity->ctx = ctx;
  activity->next = NULL;
  activity->prev = prev;
  activity->type = ACTIVITY_TYPE_SANDBOX;
  activity->phase = ACTIVITY_PHASE_ACTIVE;
  activity->details = malloc(sizeof(activity_sandbox_t));

  if (prev != NULL) {
    prev->next = activity;
  }

  return activity;
}

void activity_sandbox_on_resize(activity_t* activity, int rows, int cols) {
  char msg[80];
  sprintf(msg, "RESIX %dx%d\n\r", rows, cols);
  wbkgd(stdscr, COLOR_PAIR(MY_PAIR_DESKTOP));


  WINDOW* win = ((activity_sandbox_t*)(activity->details))->redwin;
  delwin(win);

  win = newwin(15, 37, 2, 10);
  ((activity_sandbox_t*)(activity->details))->redwin = win;

  wbkgd(win, COLOR_PAIR(MY_PAIR_ALERT));
  refresh();
  box(win, 0, 0);

  wbkgd(win, COLOR_PAIR(MY_PAIR_ALERT));
  mvwprintw(win, 0, 1, "Greeter");
  mvwprintw(win, 1, 1, "Hello");
  waddstr(win, msg);

  // refreshing the window
  wrefresh(win);
  
}

void activity_sandbox_on_keypress(activity_t* activity, int key) {

  WINDOW* win = ((activity_sandbox_t*)(activity->details))->redwin;
  wattron(win, COLOR_PAIR(MY_PAIR_LABEL));
  char msg[80];
  sprintf(msg, "sb %d,\n\r", key);
  waddstr(win, msg);

	wrefresh(win);

  if (key == 27) {
    sprintf(msg, "Exiting book sandbox %d", key);
    activity->phase = ACTIVITY_PHASE_COMPLETE;
    wbkgd(stdscr, COLOR_PAIR(MY_PAIR_LABEL));
  }
}

// Welcome 
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
  if (key == 's') {
    activity->next = activity_sandbox_ctor(activity, activity->ctx);
    activity->phase = ACTIVITY_PHASE_BACKGROUND;
  }
}

/// SelectBook //////// 
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

// Absract methods

void virtual_activity_on_resize(activity_t* activity, int rows, int cols) {
  char ch[80];
  switch (activity->type) {
    case ACTIVITY_TYPE_WELCOME:
      activity_welcome_on_resize(activity, rows, cols);
      break;
    case ACTIVITY_TYPE_SELECTBOOK:
      activity_selectbook_on_resize(activity, rows, cols);
      break;
    case ACTIVITY_TYPE_SANDBOX:
      activity_sandbox_on_resize(activity, rows, cols);
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
    case ACTIVITY_TYPE_SANDBOX:
      activity_sandbox_on_keypress(activity, key);
      break;
    default:
      sprintf(ch, "Keypress not implemented for %d", activity->type);
      addstr(ch);
      break;
  }
}

