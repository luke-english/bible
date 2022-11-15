#ifdef EMSCRIPTEN
#include <emscripten.h>
EM_JS(int, addstr, (const char*));
#endif /* EMSCRIPTEN*/
