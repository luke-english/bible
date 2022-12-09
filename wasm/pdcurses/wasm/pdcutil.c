/* Public Domain Curses */

#include <emscripten.h>
#include "pdcwasm.h"

void PDC_beep(void)
{
    PDC_LOG(("PDC_beep() - called\n"));
    /* FIXME */
}

void PDC_napms(int ms)
{
    await_timeout(ms);

    PDC_LOG(("PDC_napms() - called: ms=%d\n", ms));
}

const char *PDC_sysname(void)
{
    return "wasm";
}

EM_JS(void, await_timeout, (int ms));