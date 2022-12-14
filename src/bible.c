#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <errno.h>

#include "program.h"
#include "ctx.h"
#include "activity.h"

#include "zcurses.h"

#include "colors.h"

uint8_t* bblob = NULL;
size_t bblob_size = 0;

ctx_t* ctx;
program_t* program;

void on_resize(int rows, int cols);

void my_handle_keypress(int ch) {
  int rows; int cols;
  if (ch == KEY_RESIZE) {

    getmaxyx(stdscr, rows, cols);
    program_on_resize(program, rows, cols);

  } else {
    program_on_keypress(program, ch);
  }
}

WINDOW* ACTWIN;
void on_init() {
  setlocale(LC_ALL, "");
  initscr();
  noecho();

  ctx = ctx_ctor();
  activity_t* activity = activity_altdata_ctor(NULL, ctx);
  program = program_ctor(activity, ctx);

  start_color();

  init_pair(MY_PAIR_LABEL, COLOR_YELLOW, COLOR_BLACK);
  init_pair(MY_PAIR_WRONG, COLOR_BLACK, COLOR_RED);
  init_pair(MY_PAIR_HINT, COLOR_WHITE, COLOR_BLACK);
  init_pair(MY_PAIR_DONE, COLOR_GREEN, COLOR_BLACK);
  init_pair(MY_PAIR_MENU, COLOR_GREEN, COLOR_BLACK);
  init_pair(MY_PAIR_MENU_SELECTED, COLOR_BLACK, COLOR_GREEN);
  init_pair(MY_PAIR_MENU_SELECTED_HI, COLOR_BLUE, COLOR_YELLOW);
  init_pair(MY_PAIR_DESKTOP, COLOR_BLUE, COLOR_WHITE);
  init_pair(MY_PAIR_ALERT, COLOR_WHITE, COLOR_RED);

  int rows; int cols;
  getmaxyx(stdscr, rows, cols);
  program_on_resize(program, rows, cols);

  // Starts the main event loop
  while (TRUE) {
    int ch = wgetch(ACTWIN);
    my_handle_keypress(ch);
  }
}

#define PAGE_SIZE 24

void load_altdata() {
  FILE *fp = stdin;

  bblob = malloc(PAGE_SIZE);
  size_t memory = PAGE_SIZE;
  int32_t ch;
  while ((ch = fgetc(fp)) != EOF && ch != 255) {
    if (bblob_size == memory - 1) {
        memory += PAGE_SIZE;
        bblob = (uint8_t *) realloc(bblob, memory);
    }
    bblob[bblob_size] = ch;
    bblob_size++;
  }  
}

#ifdef __EMSCRIPTEN__

#include <emscripten.h>

__attribute__((used)) 
void wasm_init() {
  load_altdata();
  on_init();
}

#else

#include <getopt.h>

static struct option const longopts[] = {
    { "stdin", optional_argument, NULL, 'i' },
    { NULL, 0, NULL, 0 }
};

int main(int argc, char** argv) {
  int selected_opt = getopt_long(argc, argv, "+i:", longopts, NULL);

  if (selected_opt == 'i') {
    load_altdata();
  }

  freopen("/dev/tty", "rw", stdin);
  fflush(stdin);

  on_init();
	endwin();
}
#endif
