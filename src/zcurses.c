#ifdef __EMSCRIPTEN__
#include <emscripten.h>
EM_JS(int, addstr, (const char*));
EM_JS(int, wattron, (void* win, const char*));
EM_JS(int, wattroff, (void* win, const char*));
EM_JS(void, refresh, ());
EM_JS(void, start_color, ());
EM_JS(int, init_pair, (int pair, int bg, int fg));
#endif /* EMSCRIPTEN*/
