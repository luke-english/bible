/* Public Domain Curses */

#include <curspriv.h>

#include <stdlib.h>
#include <stdio.h>

#include <emscripten.h>

EM_JS(void, js_curses_scr_close, (void));
EM_JS(void, js_curses_scr_open, (void));
EM_JS(void, js_curses_resize_screen, (const int nrows, const int ncols));


/* COLOR_PAIR to attribute encoding table. */

static struct {short f, b;} atrtab[PDC_COLOR_PAIRS];

/*
Returns TRUE if init_color() and color_content() give meaningful
results, FALSE otherwise. Called from can_change_color().
*/
bool PDC_can_change_color(void)
{
    return TRUE;
}

/*
The core of color_content(). This does all the work of that function,
except checking for values out of range and null pointers.
*/
int PDC_color_content(short color, short *red, short *green, short *blue)
{
    char str[8];
    int r, g, b;

    // TODO not needed for now
    // asm("TermGlobals.getColorString(%0);"
    //    :"=r"(color) :"r"(str));

    sscanf(str, "#%02x%02x%02x", &r, &g, &b);

    *red = DIVROUND(r * 1000, 255);
    *green = DIVROUND(g * 1000, 255);
    *blue = DIVROUND(b * 1000, 255);

    return OK;
}

/*
The core of init_color(). This does all the work of that function,
except checking for values out of range.
*/
int PDC_init_color(short color, short red, short green, short blue)
{
    char str[8];

    int r = DIVROUND(red * 255, 1000);
    int g = DIVROUND(green * 255, 1000);
    int b = DIVROUND(blue * 255, 1000);

    sprintf(str, "#%02x%02x%02x", r, g, b);
    // asm("TermGlobals.setColor(%0, %1);"
    //    :"=r"(color) :"r"(str));

    // TODO not needed for now

    wrefresh(curscr);

    return OK;
}

/*
The non-portable functionality of reset_prog_mode() is handled here --
whatever's not done in _restore_mode(). In current ports: In OS/2, this
sets the keyboard to binary mode; in Windows console, it enables or
disables the mouse pointer to match the saved mode; in others it does
nothing.
*/
void PDC_reset_prog_mode(void)
{
    PDC_LOG(("PDC_reset_prog_mode() - called.\n"));
}

/*
The same thing, for reset_shell_mode(). In OS/2 and Windows console, it
restores the default console mode; in others it does nothing.
*/
void PDC_reset_shell_mode(void)
{
    PDC_LOG(("PDC_reset_shell_mode() - called.\n"));
}

/*
This does the main work of resize_term(). It may respond to non-zero
parameters, by setting the screen to the specified size; to zero
parameters, by setting the screen to a size chosen by the user at
runtime, in an unspecified way (e.g., by dragging the edges of the
window); or both. It may also do nothing, if there's no appropriate
action for the platform.
*/
int PDC_resize_screen(int nlines, int ncols)
{
    return OK;
}

/*
Called from _restore_mode() in kernel.c, this function does the actual
mode changing, if applicable. Currently used only in DOS and OS/2.
*/
void PDC_restore_screen_mode(int i)
{
//    asm("term.restoreScreen();"); /* FIXME */
}

/*
Called from _save_mode() in kernel.c, this function saves the actual
screen mode, if applicable. Currently used only in DOS and OS/2.
*/
void PDC_save_screen_mode(int i)
{
//    asm("term.backupScreen();"); /* FIXME */
}

/*
The platform-specific part of endwin(). It may restore the image of the
original screen saved by PDC_scr_open(), if the PDC_RESTORE_SCREEN
environment variable is set; either way, if using an existing terminal,
this function should restore it to the mode it had at startup, and move
the cursor to the lower left corner. (The X11 port does nothing.)
*/
void PDC_scr_close(void)
{
    PDC_LOG(("PDC_scr_close() - called\n"));
    // EM_ASM(term.close());
    //  TODO..

    js_curses_scr_close();
}

/*
Free any memory allocated by PDC_scr_open(). Called by delscreen().
*/
void PDC_scr_free(void)
{
    if (SP)
        free(SP);
}

/*
The platform-specific part of initscr(). It must initialize acs_map[]
(unless it's preset) and several members of SP, including mouse_wait,
orig_attr (and if orig_attr is TRUE, orig_fore and orig_back), mono,
_restore and _preserve. If using an existing terminal, and the
environment variable PDC_RESTORE_SCREEN is set, this function may also
store the existing screen image for later restoration by
PDC_scr_close().
*/
int PDC_scr_open(void)
{
    PDC_LOG(("PDC_scr_open() - called\n"));

    SP = calloc(1, sizeof(SCREEN));

    if (!SP)
        return ERR;

    // EM_ASM(
    //     term = new Terminal({
    //         termDiv: 'termDiv',
    //         handler: function() {},
    //         x: 0, y: 0,
    //         initHandler: function() {
    //             term.charMode = true;
    //             term.lock = false;
    //             term.cursorOn();
    //         }
    //     });
    //     term.open();
    // );

    js_curses_scr_open();

    SP->mono = 0;
    SP->orig_attr = FALSE;

    const char *PDC_LINES = getenv("PDC_LINES"),
	       *PDC_COLS = getenv("PDC_COLS");
    if (PDC_COLS && PDC_LINES) {
        // EM_ASM_INT({
        //     term.resizeTo($0, $1);
        // }, atoi(PDC_COLS), atoi(PDC_LINES));
        // TODO 

        js_curses_resize_screen(atoi(PDC_LINES), atoi(PDC_COLS));
    }

    SP->lines = PDC_get_rows();
    SP->cols = PDC_get_columns();

    /* FIXME: we could actually beep */
    SP->audible = FALSE;

    PDC_reset_prog_mode();

    return OK;
}

// void PDC_init_pair(short pair, short fg, short bg)
// {
//     atrtab[pair].f = fg;
//     atrtab[pair].b = bg;
// }

// int PDC_pair_content(short pair, short *fg, short *bg)
// {
//     *fg = atrtab[pair].f;
//     *bg = atrtab[pair].b;

//     return OK;
// }

