import { Terminal } from 'xterm';
import { FitAddon } from 'xterm-addon-fit';
import { curses } from './curses.js';
import * as boilerplate from './boilerplate.js';

const ctx = {};

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

console.log(boilerplate);
var asmLibraryArg = {
  memory: new WebAssembly.Memory({
    initial: 2, // x 64KB
    maximum: 10, // 640K ought to be enough for anybody!
    shared: false
  }),
  table: new WebAssembly.Table({ initial: 256, element: 'anyfunc' }),
  ...boilerplate,
  ...curses(ctx),
};

var importObject = {
  "env": asmLibraryArg,
  "wasi_snapshot_preview1": asmLibraryArg
};

fetch("./wasm/bible.wasm")
  .then((response) => response.arrayBuffer())
  .then((bytes) => WebAssembly.instantiate(bytes, importObject))
  .then((module) => {
    ctx.buffer = module.instance.exports.memory.buffer;
    ctx.term = createTerminal();

    ctx.term.onResize(({ cols, rows }) => {
      module.instance.exports.on_resize(rows, cols);
    });

    ctx.term.onKey((keyEvt) => {
      const { key } = keyEvt;
      const chars = new TextEncoder().encode(key);
      chars.forEach(c => {
        module.instance.exports.on_keypress(c);
      });
    });

    module.instance.exports.on_init();
    module.instance.exports.on_resize(ctx.term.rows, ctx.term.cols);
  });
