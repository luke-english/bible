/* Public Domain Curses */

#include "pdcwasm.h"

/* get the cursor size/shape */

int PDC_get_cursor_mode(void)
{
    PDC_LOG(("PDC_get_cursor_mode() - called\n"));

    return js_curses_get_cursor_mode();

    // TODO
    // return EM_ASM_INT_V({
    //     return term.crsrBlinkMode ? 0 : term.crsrBlockMode ? 1 : 2;
    // });
}

/* return number of screen rows */

int PDC_get_rows(void)
{
    PDC_LOG(("PDC_get_rows() - called\n"));

    return js_curses_get_rows();
    // TODO
    /* FIXME: should return the rows that fit into termDiv */
    // return EM_ASM_INT_V({
    //     return term.conf.rows;
    // });
}

/* return width of screen/viewport */

int PDC_get_columns(void)
{
    PDC_LOG(("PDC_get_columns() - called\n"));

    return js_curses_get_cols();

    // TODO 
    // /* FIXME: should return the cols that fit into termDiv */
    // return EM_ASM_INT_V({
    //     return term.conf.cols;
    // });
}
