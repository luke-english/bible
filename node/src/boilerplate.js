
function writeAsciiToMemory(str, buffer, dontAddNull) {
  
}

function getExecutableName() {
  return "./this.program";
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

    var strings = [];
    for (var x in env) {
      strings.push(x + "=" + env[x]);
    }
    getEnvStrings.strings = strings;
  }
  return getEnvStrings.strings;
}

export function environ_get(__environ, environ_buf) {
  return 0;
}

export function environ_sizes_get(penviron_count, penviron_buf_size) {
  return 0;
}

export function fd_close(fd) {
  return 52;
}

export function fd_seek(fd, offset_low, offset_high, whence, newOffset) {
  console.log("supposed to fd_seek");

  return 70;
}

export function fd_write(fd, iov, iovcnt, pnum) {
  console.log("supposed to fd_write", fd, iov, iovcnt, pnum);
  throw 2;
}

export function proc_exit(code) {
  return 0;
}