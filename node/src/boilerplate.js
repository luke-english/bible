
function writeAsciiToMemory(str, buffer, dontAddNull) {
}

function getExecutableName() {
  return thisProgram || "./this.program";
}

function getEnvStrings() {
  if (!getEnvStrings.strings) {
    var lang = (typeof navigator == "object" && navigator.languages && navigator.languages[0] || "C").replace("-", "_") + ".UTF-8";
    var env = {
      "USER": "web_user",
      "LOGNAME": "web_user",
      "PATH": "/",
      "PWD": "/",
      "HOME": "/home/web_user",
      "LANG": lang,
      "_": getExecutableName()
    };
    for (var x in ENV) {
      if (ENV[x] === undefined) delete env[x]; else env[x] = ENV[x];
    }
    var strings = [];
    for (var x in env) {
      strings.push(x + "=" + env[x]);
    }
    getEnvStrings.strings = strings;
  }
  return getEnvStrings.strings;
}

export function environ_get(__environ, environ_buf) {
  var bufSize = 0;
  getEnvStrings().forEach(function (string, i) {
    var ptr = environ_buf + bufSize;
    HEAPU32[__environ + i * 4 >> 2] = ptr;
    writeAsciiToMemory(string, ptr);
    bufSize += string.length + 1;
  });
  return 0;
}

export function environ_sizes_get(penviron_count, penviron_buf_size) {
  var strings = getEnvStrings();
  HEAPU32[penviron_count >> 2] = strings.length;
  var bufSize = 0;
  strings.forEach(function (string) {
    bufSize += string.length + 1;
  });
  HEAPU32[penviron_buf_size >> 2] = bufSize;
  return 0;
}

export function fd_close(fd) {
  return 52;
}

export function fd_seek(fd, offset_low, offset_high, whence, newOffset) {
  return 70;
}

export function fd_write(fd, iov, iovcnt, pnum) {
  console.log("supposed to fd_write", fd, iov, iovcnt, pnum);
  throw 2;
}

export function proc_exit(code) {
  EXITSTATUS = code;
  quit_(code, new ExitStatus(code));
}