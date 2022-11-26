#include <stdlib.h>
#include <stdio.h>
#include <locale.h>


#include "program.h"
#include "ctx.h"
#include "activity.h"

#include "zcurses.h"
#include "colors.h"

ctx_t* ctx;
program_t* program;

__attribute__((used))
void on_init() {
  int rows; int cols;

  setlocale(LC_ALL, "");
  initscr();
  noecho();

  ctx = ctx_ctor();
  activity_t* activity = activity_welcome_ctor(NULL, ctx);
  program = program_ctor(activity, ctx);

  start_color();

  init_pair(MY_PAIR_LABEL, COLOR_YELLOW, COLOR_BLACK);
  init_pair(MY_PAIR_WRONG, COLOR_BLACK, COLOR_RED);
  init_pair(MY_PAIR_TODO, COLOR_WHITE, COLOR_BLACK);
  init_pair(MY_PAIR_DONE, COLOR_GREEN, COLOR_BLACK);
  init_pair(MY_PAIR_MENU, COLOR_GREEN, COLOR_BLACK);
  init_pair(MY_PAIR_MENU_SELECTED, COLOR_BLACK, COLOR_GREEN);
  init_pair(MY_PAIR_MENU_SELECTED_HI, COLOR_BLUE, COLOR_YELLOW);
  init_pair(MY_PAIR_DESKTOP, COLOR_BLUE, COLOR_WHITE);
  init_pair(MY_PAIR_ALERT, COLOR_WHITE, COLOR_RED);
}

__attribute__((used))
void on_resize(int rows, int cols) {
  program_on_resize(program, rows, cols);
}

__attribute__((used))
int on_keypress(int key) {
  program_on_keypress(program, key);
  return 1;
}

#ifndef __EMSCRIPTEN__
int main(int argc, char** argv) {
  int rows; int cols;

  on_init();
  getmaxyx(stdscr, rows, cols);
  on_resize(rows, cols);

  char ch;
  while (TRUE) {
    ch = getch();

    if (ch == 154) {
      getmaxyx(stdscr, rows, cols);
      on_resize(rows, cols);
    } else {
      on_keypress(ch);
    }
  }

	endwin();
}
#endif
