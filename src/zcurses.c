#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include "zcurses.h"

EM_JS(int, js_curses_check_key, ());
EM_JS(void, js_curses_set_char, (int a, int b, int c, int d));
EM_JS(int, js_curses_get_key, ());
EM_JS(int, js_curses_get_rows, ());
EM_JS(int, js_curses_get_cursor_mode, ());
EM_JS(int, js_curses_get_cols, ());
EM_JS(void, js_curses_flushinp, ());
EM_JS(void, js_curses_scr_close, ());
EM_JS(void, js_curses_scr_open, ());
EM_JS(void, js_curses_curs_on, ());
EM_JS(void, js_curses_curs_off, ());
EM_JS(void, js_curses_simulateinfiniteloop_error, ());
EM_JS(void, js_curses_resize_screen, (const int nrows, const int ncols));
EM_JS(void, js_curses_gotoyx, (int,int));

#endif /* EMSCRIPTEN*/
