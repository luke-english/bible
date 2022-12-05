#pragma once

// PDCurses for WASM are not installed to OS, therefore quotes
#ifdef __EMSCRIPTEN__
#include "curses.h"
#include "pdcwasm.h"
#endif /* EMSCRIPTEN */

// For Alpine we just use OS package, therefore angles
#ifndef __EMSCRIPTEN__
#include <curses.h>
#endif /* !EMSCRIPTEN */

