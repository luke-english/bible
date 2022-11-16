#include <stdlib.h>
#include <stdio.h>

#include "program.h"
#include "ctx.h"
#include "activity.h"

ctx_t* ctx;
program_t* program;

__attribute__((used))
void on_init() {
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

int main(int argc, char** argv) {
  // on_init();
}

