/* Public Domain Curses */

/*man-start**************************************************************

  Name:                                                         pdckbd

  Synopsis:
        unsigned long PDC_get_input_fd(void);

  Description:
        PDC_get_input_fd() returns the file descriptor that PDCurses 
        reads its input from. It can be used for select().

  Portability                                X/Open    BSD    SYS V
        PDC_get_input_fd                        -       -       -

**man-end****************************************************************/

#include <string.h>

#include "pdcwasm.h"

void PDC_set_keyboard_binary(bool on)
{
    PDC_LOG(("PDC_set_keyboard_binary() - called\n"));
}

/* check if a key or mouse event is waiting */

bool PDC_check_key(void)
{
    if (PDC_get_columns() != SP->cols || PDC_get_rows() != SP->lines) {

        SP->lines = PDC_get_rows();
        SP->cols = PDC_get_columns();
        SP->resized = FALSE;

        return TRUE;
    }

    return js_curses_check_key() ? TRUE : FALSE;
}

/* return the next available key or mouse event */

int PDC_get_key(void)
{
    int key = js_curses_get_key();

    SP->key_code = (key > 0x100);

    if (!key) {
        if (!SP->resized) {
            SP->resized = TRUE;
            key = KEY_RESIZE;
        }
    }

    return key ? key : -1;
}

/* discard any pending keyboard or mouse input -- this is the core
   routine for flushinp() */

void PDC_flushinp(void)
{
    PDC_LOG(("PDC_flushinp() - called\n"));

    js_curses_flushinp();
}

int PDC_mouse_set(void)
{
    return ERR;
}

int PDC_modifiers_set(void)
{
    return OK;
}
