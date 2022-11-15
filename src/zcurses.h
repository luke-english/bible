#pragma once

#ifdef EMSCRIPTEN

int    addstr(const char *);

#endif /* EMSCRIPTEN */
#ifndef EMSCRIPTEN

#include <curses.h>

#endif /* !EMSCRIPTEN */

