#include "pdcwasm.h"

typedef void global_cb_t(int ch);

global_cb_t* global_cb = NULL;

void _set_cb(global_cb_t* cb) {
  global_cb = cb;
}

void ingest_ch(int ch) {
  global_cb(ch);
}

void wgetch_async(WINDOW* win, global_cb_t* cb) {
#ifdef __EMSCRIPTEN__
  _set_cb(cb);
#else
  // traditional way of doing it
  while (TRUE) {
    int ch = wgetch(win);
    cb(ch);
  }
#endif
}