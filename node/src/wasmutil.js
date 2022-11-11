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

export { encode, decode }
