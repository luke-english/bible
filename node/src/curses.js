const curses = (ctx) => {

  const js_curses_set_char = (ch, row, col, style) => {
    console.log(`js_curses_set_char()`, {ch, row, col, style});

    pair = [style >> 8];
    const fg = style >> 8;
    const bg = 0;

    start = `\x1B[4${bg};3${fg}m`;
    end = '\x1B[0m';
    ctx.term.write(`${start}${String.fromCharCode(ch)}${end}`);
  }
  const js_curses_get_rows = () => {
    console.log("js_curses_get_rows");
    return 25;
  }

  const js_curses_get_cols = () => {
    console.log("js_curses_get_cols");
    return 80;
  }

  const js_curses_curs_on = () => {
    console.log("js_curses_curs_on");
  }

  const js_curses_curs_off = () => {
    console.log("js_curses_curs_off");
  }

  const js_curses_resize_screen = (nrows, ncols) => {
    console.log("js_curses_resize_screen");
  }

  const js_curses_gotoyx = (row, col) => {
    console.log(`gotoyx(${row}x${col})`);
  }

  const js_curses_get_cursor_mode = () => {
    console.log("js_curses_get_cursor_mode");
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

