// import { ansiEscapes } from 'ansi-escapes';
import ansi from 'ansi-escape-sequences'

const curses = (ctx) => {

  const js_curses_set_char = (ch, row, col, fg, bg) => {
    console.log(`js_curses_set_char()`, {ch, row, col, fg, bg});

    // Terminal colors:
    //
    // 0 Black  2 Green   4 Blue     6 Cyan    
    // 1 Red    3 Yellow  5 Magenta  7 White   
    
    // For some reason PDCurses returns colors mixed
    // Curses:     0  1  2  3  4  5  6  7  8      Actual values:
    const fMap = [ 0, 4, 2, 3, 4, 5, 3, 7, 8 ] // Foreground
    const bMap = [ 9, 1, 2, 3, 1, 5, 3, 7, 8 ] // Background

    let b = bMap[bg]
    let f = fMap[fg]

    // Just in case
    if (b == f) f++;

    start = `\x1B[4${b};3${f}m`;
    end = '\x1B[0m';
    ctx.term.write(`${start}${String.fromCharCode(ch)}${end}`);
  }
  const js_curses_get_rows = () => {
    return ctx.term.rows;
  }

  const js_curses_get_cols = () => {
    return ctx.term.cols;
  }

  const js_curses_curs_on = () => {
    ctx.term.write(ansi.cursor.show);
  }

  const js_curses_curs_off = () => {
    ctx.term.write(ansi.cursor.hide);
  }

  const js_curses_resize_screen = (nrows, ncols) => {
    console.log("js_curses_resize_screen how can it be?");
  }

  const js_curses_gotoyx = (row, col) => {
    ctx.term.write(ansi.cursor.position(row, col))
  }

  const js_curses_get_cursor_mode = () => {
    console.log("js_curses_get_cursor_mode");
    // ctx.term.getOption('cursorStyle') ? 1 : 0;
    // TODO
  }

  const js_curses_scr_open = () => {
    console.log("js_curses_scr_open");
  }

  return {
    js_curses_set_char,
    js_curses_get_rows,
    js_curses_get_cols,
    js_curses_curs_on,
    js_curses_curs_off,
    js_curses_resize_screen,
    js_curses_gotoyx,
    js_curses_get_cursor_mode,
    js_curses_scr_open,
  };
}

export { curses };

