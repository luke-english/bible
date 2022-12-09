import { Terminal } from 'xterm';
import { FitAddon } from 'xterm-addon-fit';
import { curses } from './curses.js';
import * as boilerplate from './boilerplate.js';
import * as Asyncify from 'asyncify-wasm';

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
  ...boilerplate,
  ...curses(ctx),
};

var importObject = {
  "env": asmLibraryArg,
  "wasi_snapshot_preview1": asmLibraryArg
};

fetch("./wasm/bible.wasm")
  .then((response) => response.arrayBuffer())
  .then((bytes) => Asyncify.instantiate(bytes, importObject))

  .then((module) => {
    ctx.buffer = module.instance.exports.memory.buffer;
    ctx.term = createTerminal();
    ctx.term.focus();
    ctx.inputchar = null;

    // Probably not needed too...
    ctx.term.onResize(({ cols, rows }) => {
      ctx.term.rows = rows;
      ctx.term.cols = cols;
    });

    ctx.term.onKey((keyEvt) => {
      const { key } = keyEvt;
      const chars = new TextEncoder().encode(key);
      chars.forEach(c => {
        ctx.inputchar = c;
      });
    });

    module.instance.exports.on_init();
  });
