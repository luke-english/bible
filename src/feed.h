#pragma once

typedef struct feed_t feed_t;
typedef struct lines_t lines_t;

// void feed_get_last_char(feed, last_char);
// bool is_last feed_is_cursor_at_bottom(feed);
// void feed_inc_pos(feed)
// void feed_dec_pos(feed);

// int success feed_scroll_up(feed, wheight, wwidth);
// int success feed_scroll_down(feed, wheight, wwidth);

feed_t* feed_ctor(uint8_t* data, size_t blen);
lines_t* lines_ctor();
size_t feed_get_visible_lines(
    feed_t* feed,
    int height,
    int width,
    lines_t* lines
);

uint8_t* lines_take(lines_t* lines);

/*
Length of a string can be measured in 3 different units:

- bytes (blen): number of bytes;
- characters (clen): textual characters;
- positions (plen): the number of positions it moves cursor.

on keypress:

bool is_return = feed_inc_pos(feed)
if is_return && feed_is_cursor_at_bottom()
{
    feed_get_visible_lines(feed, lines));
    int y;
    uint8_t* line;
    for (y = 0, blen = lines_get(line) > -1; y < wheight; y++) {
        wmvaddchr(win, line, y, 0);
    }
    wmove(win, y, 0);
}
feed_get_last_char(feed, last_char);
waddch(win, last_char);
}

on backspace:

if (x > 0) {
    feed_dec_pos(feed);
    wmvaddch(win, "_", y, --x);
    wmove(win, y, x);
} else {}

on resize:

feed_recut(feed, wheight, wwidth);
feed_get_visible_lines(feed, lines));
int y;
for (y = 0, mvaddchr(lines[y], 0, 0); y < wheight; y++) {
    wmvaddchr(win, lines[y], y, 0);
}

On_scrollup
on_scroolldown TBD

*/

// feed_t* feed_normal_ctor(uint8_t* data, size_t size) {
//   feed_t* feed = malloc(sizeof(feed_t));
//   feed->size = size;
//   feed->cursor = 0;
//   feed->width = 0;
//   feed->height = 0;

//   for each in data


// }

// void feed_on_init(program_t* program);

// void program_on_resize(program_t* program, int rows, int cols);

// void program_on_keypress(program_t* program, int key);

