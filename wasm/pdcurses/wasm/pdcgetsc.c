/* Public Domain Curses */

#include "pdcwasm.h"

/* get the cursor size/shape */

int PDC_get_cursor_mode(void)
{
    PDC_LOG(("PDC_get_cursor_mode() - called\n"));

    return js_curses_get_cursor_mode();
}

/* return number of screen rows */

int PDC_get_rows(void)
{
    PDC_LOG(("PDC_get_rows() - called\n"));

    LINES = js_curses_get_rows();

    return LINES;
}

/* return width of screen/viewport */

int PDC_get_columns(void)
{
    PDC_LOG(("PDC_get_columns() - called\n"));
    
    COLS = js_curses_get_cols();

    return COLS;
}
