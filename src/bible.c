#include <stdlib.h>
#include <stdio.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
EM_JS(int, addstr, (const char*));
#endif /* EMSCRIPTEN*/

#ifndef EMSCRIPTEN
#include <curses.h>
#endif /* !EMSCRIPTEN */

#include "activity.h"
#include "ctx.h"
#include "scripture.h"

activity_t* head_activity;
activity_t* tail_activity;

ctx_t* ctx;

#ifdef EMSCRIPTEN
EMSCRIPTEN_KEEPALIVE
#endif
int main(int argc, char** argv) {
  head_activity = activity_welcome_ctor();
  tail_activity = head_activity;

#ifdef EMSCRIPTEN
  ctx = ctx_wasm_ctor(head_activity);
#endif

  ctx_virtual_init(ctx);
}

#ifdef EMSCRIPTEN
EMSCRIPTEN_KEEPALIVE
#endif
int on_resize(int rows, int cols) {
  char msg[80];
  sprintf(msg, "Value of Pi rows = %d; cols = %d", rows, cols);
  addstr(msg);
  return 1;
}

#ifdef EMSCRIPTEN
EMSCRIPTEN_KEEPALIVE
#endif
int on_keypress(int key) {

  const bookinfo_t* allbooks = get_all_books();

  int i = 0;
  int n = NUMBER_OF_BOOKS;
  for (i = 0; i < n; i++) {
    const bookinfo_t book = allbooks[i];
    char msg[20];
    sprintf(msg, "%d - %s", i+1, book.title);
    addstr(msg);
  }
  return 1;
}
