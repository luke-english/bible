#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include "pdcwasm.h"

EM_JS(void, js_curses_gotoyx, (const int row, const int col));
EM_JS(void, js_curses_set_char, (
  const int ch,
  const int row,
  const int col,
  const int fg,
  const int bg
));
EM_JS(int, js_curses_get_cursor_mode, (void));
EM_JS(int, js_curses_check_key, (void));
EM_JS(int, js_curses_get_key, (void));
EM_JS(int, js_curses_get_rows, (void));
EM_JS(int, js_curses_get_cols, (void));
EM_JS(void, js_curses_flushinp, (void));
EM_JS(void, js_curses_scr_close, (void));
EM_JS(void, js_curses_scr_open, (void));
EM_JS(void, js_curses_curs_on, (void));
EM_JS(void, js_curses_curs_off, (void));
EM_JS(void, js_curses_simulateinfiniteloop_error, (void));
EM_JS(void, js_curses_resize_screen, (const int nrows, const int ncols));
EM_JS(void, js_curses_transform_line, (int lineno, int x, int len, const void *srcp, short fg, short bg));

#endif /* EMSCRIPTEN*/
