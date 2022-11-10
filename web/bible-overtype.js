const ctx = {};

const importObject = {
  env: {
    memory: new WebAssembly.Memory({
      initial: 2, // x 64KB
      maximum: 10, // 640K ought to be enough for anybody!
      shared: false
    }),
    call_alert: (...param) => {
      console.log("with param", {param});
    },
    addstr: (str) => {
      console.log('&str=', str);
      const snapshot = new Uint8Array(ctx.buffer)
      const decoded_str = decode(snapshot, str) 

      const startRed = '\x1B[1;3;31m';
      const endColor = '\x1B[0m';
      ctx.term.write(`DECODED_ARG=${startRed}${decoded_str}${endColor};\n\r`);

      console.log("ADDSTR is called in another side with", {decoded_str});
    },
  },
};

fetch("./bible-overtype.wasm")
  .then((response) => response.arrayBuffer())
  .then((bytes) => WebAssembly.instantiate(bytes, importObject))
  .then((module) => {
    // init ctx
    ctx.buffer = module.instance.exports.memory.buffer;
    ctx.term = new Terminal();

    const keyResult = module.instance.exports.on_keypress(85);
    const sizeResult = module.instance.exports.on_resize(25, 80);

    ctx.term.open(document.getElementById('terminal'));
    ctx.term.write(` K=${keyResult}; S=${sizeResult}`);
  });

// Encode string into memory starting at address base.
const encode = (memory, base, string) => {
  for (let i = 0; i < string.length; i++) {
    memory[base + i] = string.charCodeAt(i);
  }

  memory[base + string.length] = 0;
};

// Decode a string from memory starting at address base.
const decode = (memory, base) => {
  let cursor = base;
  let result = '';

  while (memory[cursor] !== 0) {
    result += String.fromCharCode(memory[cursor++]);
  }

  return result;
};

