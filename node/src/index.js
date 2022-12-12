import { Terminal } from 'xterm';
import { FitAddon } from 'xterm-addon-fit';
import { curses } from './curses.js';
import * as boilerplate from './boilerplate.js';
import * as Asyncify from 'asyncify-wasm';

export const loadWasm = (domElement, {altdata}) => {
  const ctx = {};

  const utf8Encode = new TextEncoder();
  ctx.altdata = Array.from(utf8Encode.encode(altdata));

  const createTerminal = () => {
    const fitAddon = new FitAddon();
    const terminal = new Terminal({ scrollback: 0 });
  
    // On container resize terminal will "fit"
    terminal.loadAddon(fitAddon);
    const onWindowSize = () => {
      fitAddon.fit();
    }
    window.addEventListener('resize', onWindowSize, false);
  
    terminal.open(domElement);
    onWindowSize();
  
    return terminal;
  }
  
  var asmLibraryArg = {
    ...boilerplate,
    ...curses(ctx),
    // size_t fread(
    //   void *restrict ptr, size_t size, size_t nmemb,
    // FILE *restrict stream);
    //    fread(buffer, sizeof(char), BUFFER_SIZE, fIn);
    fd_read: (bufferptr, size, nmemb, stream) => {
      console.log('fd_read args', {bufferptr, size, nmemb, stream} )
      if (bufferptr == 0)return 0;
      throw new Error("buffer needs to be filled");
    },
    args_sizes_get: (...args) => {
      console.log("args_sizes_get args", args)
      return 1;
    },
    args_get: (...args) => {
      console.log("args_get args", args)
  
    },
    altdata_read: (ptr, chunk_size) => {
  
      console.log("altdata_read called ", ptr, chunk_size);
      const snapshot = new Uint8Array(ctx.buffer);
      let i = 0;
      console.log(ctx.altdata)
      while (i < chunk_size && ctx.altdata.length > 0) {
        const byte = ctx.altdata.shift();
        snapshot[ptr + i] = byte;
        console.log("puts byte to memory", byte, ptr + i);
        i++;
      }
      return i;
    }
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

      ctx.term.onResize(({ cols, rows }) => {
        ctx.term.cols = cols;
        ctx.term.rows = rows;
      });

      ctx.term.onKey((keyEvt) => {
        const { key } = keyEvt;
        const chars = new TextEncoder().encode(key);
        chars.forEach(c => {
          ctx.inputchar = c;
        });
      });

      module.instance.exports.wasm_init();
    });
}

