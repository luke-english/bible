#include <stdlib.h>
// #include <stdio.h>
#include <locale.h>
#include <unistd.h>

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
  if (ch == -1) {
    return; // Actually, this only happens when the user closes the active window
  }
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
  ctx_set_size(ctx, rows, cols);
  
  ACTWIN = stdscr;

  program_on_init(program);
  
  // Starts the main event loop
  while (TRUE) {
    if (ACTWIN == NULL) {
      exit(1);
    }
    int ch = wgetch(ACTWIN);
    my_handle_keypress(ch);
  }
}

// Assume stdin is very big
#define PAGE_SIZE 65536 * 100 // 64 KB x 100

void load_altdata() {
  FILE *fp = stdin;

  size_t allocated = PAGE_SIZE;
  bblob = malloc(allocated);

  int ch;
  while ((ch = fgetc(fp)) != EOF) {

    // // Should allocate more memory already?
    // if (bblob_size > allocated - 3) {
    //   allocated += PAGE_SIZE;
    //   void* tmp = realloc(bblob, allocated);
    //   browser_log("Failed to allocate memory\n\0");
    //   if (tmp == NULL) {
    //     exit(EXIT_FAILURE);
    //   }
    //   bblob = tmp;     
    // }

    bblob[bblob_size] = ch;
    bblob_size++;
  }
  // downsize to what we need
  bblob = (uint8_t *) realloc(bblob, bblob_size+1);
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
