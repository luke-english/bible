#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include "zcurses.h"
EM_JS(int, addstr, (const char*));
EM_JS(int, waddstr, (void*, const char*));
EM_JS(void, refresh, ());
EM_JS(void, start_color, ());
EM_JS(int, init_pair, (int pair, int bg, int fg));
EM_JS(void, mvwprintw, (WINDOW*, int, int, const char*));
EM_JS(void, wrefresh, (void*));
EM_JS(void, wattroff, (WINDOW*, chtype));
EM_JS(void, wattron, (WINDOW*, chtype));
EM_JS(void, wbkgd, (void*, int));
EM_JS(void, box, (WINDOW*, int, int));
EM_JS(WINDOW*, newwin, (int, int, int, int));
EM_JS(void, delwin, (WINDOW*));
#endif /* EMSCRIPTEN*/
