#pragma once

#ifdef __EMSCRIPTEN__


#define COLOR_BLACK	0
#define COLOR_RED	1
#define COLOR_GREEN	2
#define COLOR_YELLOW	3
#define COLOR_BLUE	4
#define COLOR_MAGENTA	5
#define COLOR_CYAN	6
#define COLOR_WHITE	7


#define stdscr 0 

typedef uint32_t chtype;
#define NCURSES_CAST(type,value) (type)(value)
#define NCURSES_ATTR_SHIFT       8
#define NCURSES_BITS(mask,shift) (NCURSES_CAST(chtype,(mask)) << ((shift) + NCURSES_ATTR_SHIFT))
#define A_COLOR		NCURSES_BITS(((1U) << 8) - 1U,0)
#define COLOR_PAIR(n)	(NCURSES_BITS((n), 0) & A_COLOR)


colorPair = (n) => (
    NCURSES_BITS((n), 0) (n << 8))
    & A_COLOR)

int init_pair(int pair, int bg, int fg);
int wattron(void* win, int attr);
int wattroff(void* win, int attr);
int addstr(const char *);

void refresh();
void start_color();

#endif /* EMSCRIPTEN */

#ifndef __EMSCRIPTEN__
#include <curses.h>
#endif /* !EMSCRIPTEN */

