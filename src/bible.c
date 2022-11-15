#include <stdlib.h>
#include <stdio.h>

#include "program.h"
#include "ctx.h"
#include "activity.h"
#include "scripture.h"

ctx_t* ctx;
program_t* program;

__attribute__((used))
void on_init() {
  activity_t* activity = activity_welcome_ctor(ctx);
  program = program_ctor(activity, ctx);
}

__attribute__((used))
void on_resize(int rows, int cols) {
  program_on_resize(program, rows, cols);
}

__attribute__((used))
int on_keypress(int key) {
  program_on_keypress(program, key);

  const bookinfo_t* allbooks = get_all_books();

  int i = 0;
  int n = NUMBER_OF_BOOKS;
  for (i = 0; i < n; i++) {
    const bookinfo_t book = allbooks[i];
    char msg[20];
    sprintf(msg, "%d - %s", i+1, book.title);
    // addstr(msg);
  }
  return 1;
}

int main(int argc, char** argv) {
  // on_init();
}

