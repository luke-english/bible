import { Terminal } from 'xterm';
import { FitAddon } from 'xterm-addon-fit';
import { curses } from './curses.js';

const ctx = {};

const importObject = {
  env: {
    memory: new WebAssembly.Memory({
      initial: 2, // x 64KB
      maximum: 10, // 640K ought to be enough for anybody!
      shared: false
    }),
    ...curses(ctx),
  },
};

const createTerminal = () => {
    const fitAddon = new FitAddon();
    const terminal = new Terminal({
      scrollback: 0
    });

    // On container resize terminal will "fit"
    terminal.loadAddon(fitAddon);
    const onWindowSize = () => {
      fitAddon.fit();
    }
    window.addEventListener('resize', onWindowSize, false);

    // Open the terminal now!
    terminal.open(document.getElementById('terminal-container'));
    onWindowSize(); // trigger fit initially...

    return terminal;
}

fetch("./wasm/bible.wasm")
  .then((response) => response.arrayBuffer())
  .then((bytes) => WebAssembly.instantiate(bytes, importObject))
  .then((module) => {
    ctx.buffer = module.instance.exports.memory.buffer;
    ctx.term = createTerminal();

    ctx.term.onResize(({cols, rows}) => {
      console.log(`Terminal resize detected ${cols}x${rows}`);
      module.instance.exports.on_resize(rows, cols);
    });

    ctx.term.onKey((keyEvt) => {
      const key = keyEvt.domEvent.which
      console.log(keyEvt);
      console.log(`Terminal keypress detected ${key}`);
      const keyResult = module.instance.exports.on_keypress(key);
    });

    module.instance.exports.on_init();
  });
