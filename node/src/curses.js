import ansi from 'ansi-escape-sequences'

const curses = (ctx) => {

  const js_curses_set_char = (ch, row, col, fg, bg) => {
    // Terminal colors:
    // 0 Black  2 Green   4 Blue     6 Cyan    
    // 1 Red    3 Yellow  5 Magenta  7 White   
    //
    // 9 Color at startup
    
    // For some reason PDCurses returns colors mixed
    // Curses:     0  1  2  3  4  5  6  7  8      Actual values:
    const fMap = [ 7, 4, 2, 3, 4, 5, 3, 7, 8 ] // Foreground
    const bMap = [ 9, 1, 2, 3, 1, 5, 3, 7, 8 ] // Background

    // "Corrected" values:
    let b = bMap[bg]
    let f = fMap[fg]

    // If both zeros we assum A_COLOR is not set
    // which is probably wrong when domebody has pair black-on-black
    // but it is an edge case.
    // (TODO use different js-curses_* f-ion for default color)
    if (fg == 0 && bg == 0) f = 7;
    if (fg == -2) f = 7;
    if (bg == -2) b = 9;

    start = `\x1B[4${b};3${f}m`;
    end = '\x1B[0m';
    ctx.term.write(`${start}${String.fromCharCode(ch)}${end}`);

    console.log(`js_curses_set_char(`
      + `${String.fromCharCode(ch)}(${ch}) ${row}x${col} `
      + `fg:${fg}->${f} bg:${bg}->${b}`);
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
    ctx.term.write(ansi.cursor.hide)
  }

  const js_curses_resize_screen = (nrows, ncols) => {
    console.log("js_curses_resize_screen how can it be?")
  }

  const js_curses_gotoyx = (row, col) => {
    ctx.term.write(ansi.cursor.position(row+1, col+1))
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
