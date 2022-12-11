#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
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

// __attribute__((used)) 
void on_init() {
  // FILE *fp = stdin;
  // int memory = 10;
  // char *all = malloc(10);
  // char ch;

  // int i = 0;

  // while ((ch = fgetc(fp)) != EOF && ch != 255) {
  //     printf("BLOB !! %d\r\n", ch);

  //     if (i == memory - 1) {
  //         memory += 10;
  //         all = (char *) realloc(all, memory);
  //     }
  //     all[i] = ch;
  //     i++;
  // }

  // // printf()

  int rows; int cols;

  setlocale(LC_ALL, "");
  initscr();
  noecho();

  ctx = ctx_ctor();
  activity_t* activity = activity_altdata_ctor(NULL, ctx);
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

  // Starts the main event loop
  while (TRUE) {
    int ch = getch();
    my_handle_keypress(ch);
  }
}
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
EM_JS(size_t, altdata_read, (uint8_t* bblob, size_t chunk_size));
#endif

#define PAGE_SIZE 24

#ifdef __EMSCRIPTEN__
__attribute__((used)) 
void wasm_init() {
  size_t bytes_read = 0;
  bblob = malloc(PAGE_SIZE);
  size_t memory = 0;
  {
    memory += PAGE_SIZE;
    realloc(bblob, PAGE_SIZE);
    size_t bytes_read = altdata_read(&bblob[bytes_read], PAGE_SIZE);
    bblob_size += bytes_read;
  } while (bytes_read == PAGE_SIZE);

  on_init();
}

#else
int main(int argc, char** argv) {
  on_init();
	endwin();
}
#endif
