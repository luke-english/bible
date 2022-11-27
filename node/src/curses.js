import ansi from 'ansi-escape-sequences'

const curses = (ctx) => {

  // Terminal colors:
  // 0 Black  2 Green   4 Blue     6 Cyan    
  // 1 Red    3 Yellow  5 Magenta  7 White   
  //
  // 9 Color at startup
  
  // For some reason PDCurses returns colors mixed
  // Curses:     0  1  2  3  4  5  6  7  8      Actual values:
  const fMap = [ 0, 4, 2, 3, 4, 5, 3, 7, 8 ] // Foreground
  const bMap = [ 9, 1, 2, 3, 1, 5, 3, 7, 8 ] // Background

  const _get_color_ansi_code_fragments = (fg, bg) => {

    // console.log(`_get_color_ansi_code_fragments(`
    //   + `fg:${fg}->${f} bg:${bg}->${b}`);

    let b = bMap[bg]
    let f = fMap[fg]

    // If both zeros we assum A_COLOR is not set
    // which is probably wrong when domebody has pair black-on-black
    // but it is an edge case.
    // (TODO use different js-curses_* f-ion for default color)
    if (fg == 0 && bg == 0) {
      return {start: '', end: ''}
    }
    if (fg == -1 && bg == -1) {
      return {start: '', end: ''}
    }
    if (fg == -1) f = 7;
    if (bg == -1) b = 9;

    if (b === undefined) console.error("undefined bg val:", bg);
    if (f === undefined) console.error("undefined fg val:", fg);
    start = `\x1B[4${b};3${f}m`;
    end = '\x1B[0m';
    return {start, end}
  }

  const js_curses_set_char = (ch, row, col, fg, bg) => {
    // const { start, end } = _get_color_ansi_code_fragments(fg, bg);
    // console.log({ch})

    // ctx.term.write(ansi.cursor.position(row+1, col+1))
    // ctx.term.write(start + String.fromCharCode(ch) + end);
  }

  const js_curses_transform_line = (row, col, len, str, fg, bg) => {
    // len = len * 2;
    ctx.term.write(ansi.cursor.position(row+1, col+1))

    // Decode a string from memory starting at address base.
    const decode = (memory, base, len) => {
      let cursor = base;
      const bytes = []

      while (cursor < base+len) {
        const b = memory[cursor++];
        bytes.push(b);
      }

      console.log({bytes, memory})
      const enc = new TextDecoder()
      return enc.decode(new Uint8Array(bytes));
    };
    const snapshot = new Uint8Array(ctx.buffer);
    const value = decode(snapshot, str, len);

    console.log(`transform_line(${row}x${col}, #${value} ""[${len}]", ${fg}:${bg}`);

    const { start, end } = _get_color_ansi_code_fragments(fg, bg);
    ctx.term.write(start + value + end);
  }

  const js_curses_get_rows = () => {
    console.log("get_rows ctx.term.rows", ctx.term.rows)

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
    js_curses_transform_line
  };
}

export { curses };
