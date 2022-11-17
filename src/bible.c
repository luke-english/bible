#include <stdlib.h>
#include <stdio.h>

#include "program.h"
#include "ctx.h"
#include "activity.h"

#include "zcurses.h"

ctx_t* ctx;
program_t* program;

__attribute__((used))
void on_init() {
  ctx = ctx_ctor();
  activity_t* activity = activity_welcome_ctor(NULL, ctx);
  program = program_ctor(activity, ctx);
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
	initscr();			/* Start curses mode 		  */

  on_init();
  on_resize(100, 50);

  char ch;
  while (TRUE) {
    ch = getch();

    on_keypress(ch);

	  refresh();
  }

	endwin();
}
#endif
