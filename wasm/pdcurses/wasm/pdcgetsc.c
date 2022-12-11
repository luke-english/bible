/* Public Domain Curses */

#include "pdcwasm.h"


/*
Returns the size of the screen in columns. It's used in initscr() and
resize_term() to set the value of COLS.
*/
int PDC_get_columns(void)
{
    PDC_LOG(("PDC_get_columns() - called\n"));
    
    COLS = js_curses_get_cols();

    return COLS;
}

/*
Returns the size of the screen in rows. It's used in initscr() and
resize_term() to set the value of LINES.
*/
int PDC_get_rows(void)
{
    PDC_LOG(("PDC_get_rows() - called\n"));

    LINES = js_curses_get_rows();

    return LINES;
}

/*
Returns the size/shape of the cursor. The format of the result is
unspecified, except that it must be returned as an int. This function is
called from initscr(), and the result is stored in SP->orig_cursor,
which is used by PDC_curs_set() to determine the size/shape of the
cursor in normal visibility mode (curs_set(1)).
*/

int PDC_get_cursor_mode(void)
{
    PDC_LOG(("PDC_get_cursor_mode() - called\n"));

    return js_curses_get_cursor_mode();
}
