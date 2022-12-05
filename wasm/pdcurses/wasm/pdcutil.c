/* Public Domain Curses */

#include "pdcwasm.h"

void PDC_beep(void)
{
    PDC_LOG(("PDC_beep() - called\n"));
    /* FIXME */
}

void PDC_napms(int ms)
{
    js_curses_napms(ms);

    PDC_LOG(("PDC_napms() - called: ms=%d\n", ms));
}

const char *PDC_sysname(void)
{
    return "wasm";
}
