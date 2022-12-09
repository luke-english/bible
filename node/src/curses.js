import ansi from 'ansi-escape-sequences'

const curses = (ctx) => {

  // Decode a string from memory starting at address base.
  const decode_n = (memory, base, len) => {
    let cursor = base;
    const bytes = []

    // This parsing is done based on:
    // http://www.readytext.co.uk/?p=1284
    while (cursor < base+len) {

      const b1 = memory[cursor++];
      const b2 = memory[cursor++];

      const b1_ = memory[cursor+1];
      const b2_ = memory[cursor+2];

      if (b2 == 0) { // One byte char ////////////////////////////////////

        bytes.push(b1);

        continue;
      }
      if (false) {}
      else if (b2 < 0b0000_1000 && b1_ == 0) { // Two byte char //////////

        const y1 = (b2 >> 2) & 1;
        const y2 = (b2 >> 1) & 1;
        const y3 = (b2 >> 0) & 1;

        const y4 = (b1 >> 7) & 1;
        const y5 = (b1 >> 6) & 1;
        const x1 = (b1 >> 5) & 1;
        const x2 = (b1 >> 4) & 1;
        const x3 = (b1 >> 3) & 1;
        const x4 = (b1 >> 2) & 1;
        const x5 = (b1 >> 1) & 1;
        const x6 = (b1 >> 0) & 1;

        const a = 0b11000000 + y1*16 + y2*8  + y3*4 + y4*2 + y5*1;
        const z = 0b10000000 + x1*32 + x2*16 + x3*8 + x4*4 + x5*2 + x6*1

        bytes.push(a); // 2 bytes
        bytes.push(z);

        continue;
      } 
      else if (b2 < 0b0010_0000) { // Four byte char /////////////////////
        const www = (b2 & 0b0001_1100) >> 2;
        const zz = (b2 & 0b0000_0011) << 4;

        const zzzz = (b1 & 0b1111_0000) >> 4;
        const yyyy = (b1 & 0b0000_1111) << 2;

        const yy = (b1_ &  0b1100_0000) >> 6;
        const xxxxxx = (b1_ & 0b0011_1111) << 0;

        const w = 0b1111_0000 | www;
        const z = 0b1000_0000 | zz | zzzz;
        const y = 0b1000_0000 | yyyy | yy;
        const x = 0b1000_0000 | xxxxxx;


        bytes.push(w,z,y,x);

        /* hack requied for some emojis */
        {
          len++;
          cursor += 2;
          bytes.push(" ".charCodeAt(0));  
        }

        continue;
      }
      else if (b2 < 0b0100_0000) { // Three byte chare ////////////////////

        const z1 = (b2 >> 7) & 1;
        const z2 = (b2 >> 6) & 1;
        const z3 = (b2 >> 5) & 1;
        const z4 = (b2 >> 4) & 1;
        const y1 = (b2 >> 3) & 1;
        const y2 = (b2 >> 2) & 1;
        const y3 = (b2 >> 1) & 1;
        const y4 = (b2 >> 0) & 1;

        const y5 = (b1 >> 7) & 1;
        const y6 = (b1 >> 6) & 1;
        const x1 = (b1 >> 5) & 1;
        const x2 = (b1 >> 4) & 1;
        const x3 = (b1 >> 3) & 1;
        const x4 = (b1 >> 2) & 1;
        const x5 = (b1 >> 1) & 1;
        const x6 = (b1 >> 0) & 1;

        const z = 0b11100000 + z1*8 + z2*4 + z3*2 + z4*1;
        const y = 0b10000000 + y1*32 + y2*16 + y3*8 + y4*4 + y5*2 + y6*1;
        const x = 0b10000000 + x1*32 + x2*16 + x3*8 + x4*4 + x5*2 + x6*1;
        
        bytes.push(z, y, x);
        continue;
      }

/*
enc.encode('🍔')  [240, 159, 141, 148, ] orig 230 7
enc.encode('א')  [215, 144, ]            orig 208 5
enc.encode('ė')  [196, 151,] .           orig 23 1
enc.encode('🦄')  [240, 159, 166, 132, ] orig 23 1
*/

    }
    // console.log(`bytes;`, bytes);

    const enc = new TextDecoder()
    return enc.decode(new Uint8Array(bytes));
  };

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
    
    len *= 2;

    ctx.term.write(ansi.cursor.position(row+1, col+1))

    const snapshot = new Uint8Array(ctx.buffer);
    const value = decode_n(snapshot, str, len);

    console.log(`transform_line('${value}')[${len}];`);

    const { start, end } = _get_color_ansi_code_fragments(fg, bg);
    ctx.term.write(start + value + end);
  }

  const js_curses_get_rows = () => ctx.term.rows;

  const js_curses_get_cols = () => ctx.term.cols;

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

  js_curses_check_key = () => (ctx.inputchar !== null) ? 1 : 0;

  js_curses_get_key = () => {
    const char = ctx.inputchar;
    ctx.inputchar = null;
    return char;
  }

  const await_timeout = async (ms) => {
    // console.log('await_timeout in index.js', ms)
    return await new Promise(resolve => setTimeout(resolve, ms));
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
    js_curses_check_key,
    js_curses_get_key,
    js_curses_transform_line,
    await_timeout,
  };
}

export { curses };
