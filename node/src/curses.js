import { decode } from './wasmutil.js';

const curses = (ctx) => ({

  call_alert: (...param) => {
    console.log("with param", {param});
  },

  addstr: (str) => {
    console.log('&str=', str);
    const snapshot = new Uint8Array(ctx.buffer)
    const decoded_str = decode(snapshot, str) 

    const startRed = '\x1B[1;3;31m';
    const endColor = '\x1B[0m';
    ctx.term.write(`${startRed}${decoded_str}${endColor}\n\r`);
    console.log(`WEBCURSES: addstr("${decoded_str}")`);
  }
});

export { curses };

