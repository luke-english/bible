import { decode } from './wasmutil.js';

const curses = (ctx) => ({
  call_alert: (...param) => {
    console.log("with param", {param});
  },
  waddstr: (win, str) => {},
  addstr: (str) => {
    win = 0;
    const snapshot = new Uint8Array(ctx.buffer)
    const decoded_str = decode(snapshot, str) 

    let start = '';
    let end = '';
    if (ctx.win[win].attr) {
        const attr = ctx.win[win].attr
        pair = ctx.pairs[attr >> 8];
        const fg = pair.fg;
        const bg = pair.bg == 0 ? 9 : pair.bg;

        start = `\x1B[4${bg};3${fg}m`;
        end = '\x1B[0m';
    }
    ctx.term.write(`${start}${decoded_str}${end}`);
  },
  init_pair: (pair, fg, bg) => {
    ctx.pairs[pair] = { fg, bg };
  },
  wattron: (win, attr) => {
    ctx.win[win].attr |= attr;
  },
  wattroff(win, attr) {
    ctx.win[win].attr &= ~attr;
  },
  refresh: () => {},
  wrefresh: (win) => {},
  wbkgd: (win, colorPair) => {},
  delwin: (win) => {},
  newwin: () => {},
  box: () => {},
  mvwprintw: () => {},
  start_color: () => {
    ctx.win = [];
    ctx.pairs = [];
    ctx.win[0] = { attr: 0 };
  }
});

export { curses };

