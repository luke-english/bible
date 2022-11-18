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
typedef void WINDOW;
typedef unsigned chtype;
#define NCURSES_CAST(type,value) (type)(value)
#define NCURSES_ATTR_SHIFT       8
#define NCURSES_BITS(mask,shift) (NCURSES_CAST(chtype,(mask)) << ((shift) + NCURSES_ATTR_SHIFT))
#define A_COLOR		NCURSES_BITS(((1U) << 8) - 1U,0)
#define COLOR_PAIR(n)	(NCURSES_BITS((n), 0) & A_COLOR)


int init_pair(int pair, int bg, int fg);
void wattron(WINDOW* win, chtype attr);
void wattroff(WINDOW* win, chtype attr);
int addstr(const char *);
int waddstr(void* win, const char *);
void mvwprintw(WINDOW* win, int row, int col, const char*);
void box(WINDOW*, int, int);
void wbkgd(void* win, int colorpair);
WINDOW* newwin(int, int, int, int);

void refresh();
void wrefresh(void*);
void start_color();
void delwin(void*);

#endif /* EMSCRIPTEN */

#ifndef __EMSCRIPTEN__
#include <curses.h>
#endif /* !EMSCRIPTEN */

