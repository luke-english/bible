/* Public Domain Curses */

#include "jscurses.h"

#include <stdlib.h>
#include <string.h>
// #ifdef PDC_WIDE
# include "../common/acsuni.h"
// #else

/* draw a cursor at (y, x) */

void PDC_gotoyx(int row, int col)
{
    PDC_LOG(("PDC_gotoyx() - called: row %d col %d from row %d col %d\n",
             row, col, SP->cursrow, SP->curscol));

    js_curses_gotoyx(row, col);
}

/* update the given physical line to look like the corresponding line in
   curscr */

/* Output a block of characters with common attributes */

static int _new_packet (chtype attr, int len, int col, int lineno,
#ifdef PDC_WIDE
    uint16_t *text
#else
    unsigned char *text
#endif
) {
    short fore, back;

    pair_content(PAIR_NUMBER(attr), &fore, &back);

    /* Specify the color table offsets */

    if (attr & A_BOLD)
        fore |= 8;
    if (attr & A_BLINK)
        back |= 8;
#ifdef PDC_WIDE
    js_curses_transform_line(lineno, col, len, text, fore, back);
#else
    js_curses_transform_line(lineno, col, len, text, fore, back);
#endif

    PDC_LOG(("_new_packet() - row: %d col: %d "
             "num_cols: %d fore: %d back: %d text:<%s>\n",
             row, col, len, fore, back, text));

    return OK;
}

/* The core display routine -- update one line of text */

void PDC_transform_line(int lineno, int x, int len, const chtype *srcp)
{
#ifdef PDC_WIDE
    uint16_t text[513];
#else
    char text[513];
#endif
    chtype old_attr, attr;
    int i, j;

    PDC_LOG(("PDC_transform_line() - called: lineno: %d x: %d "
             "len: %d\n", lineno, x, len));

    if (!len)
        return;

    old_attr = *srcp & A_ATTRIBUTES;

    for (i = 0, j = 0; j < len; j++)
    {
        chtype curr = srcp[j];

        attr = curr & A_ATTRIBUTES;

        if (attr & A_ALTCHARSET && !(curr & 0xff80))
        {
            attr ^= A_ALTCHARSET;
            curr = acs_map[curr & 0x7f];
        }

#ifndef PDC_WIDE
        /* Special handling for ACS_BLOCK */

        if (!(curr & A_CHARTEXT))
        {
            curr |= ' ';
            attr ^= A_REVERSE;
        }
#endif

        if (attr != old_attr)
        {
            if (_new_packet(old_attr, i, x, lineno, text) == ERR)
                return;

            old_attr = attr;
            x += i;
            i = 0;
        }
#ifdef PDC_WIDE
        text[i++] = curr;
#else
        text[i++] = curr & 0x00ff;
#endif
    }

    _new_packet(old_attr, i, x, lineno, text);
}

void PDC_doupdate(void)
{
    PDC_napms(1);
}
