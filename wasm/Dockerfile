FROM emscripten/emsdk:3.1.25

# Fetch libunistring source code from the official FTP server.
WORKDIR /opt
RUN wget https://ftp.gnu.org/gnu/libunistring/libunistring-1.1.tar.gz
RUN tar xvzf libunistring-1.1.tar.gz

# Build and install libunistring.
WORKDIR /opt/libunistring-1.1
RUN emconfigure ./configure --prefix=/usr \
  gl_cv_func_dup2_works=no \
  ac_cv_search_nanosleep=no \
  gl_cv_func_sleep_works=no
RUN emmake make
RUN emmake make install

# Fetch patched distribution of PDCurses from version control.
WORKDIR /opt
RUN git clone --branch wasm https://github.com/luke10x/PDCurses.git pdcurses
ENV PDCURSES_SRCDIR=/opt/pdcurses

# Add WASM module for PDCurses.
ADD pdcurses/wasm /opt/pdcurses/wasm

# Build and install PDCurses.
WORKDIR /opt/pdcurses/wasm
RUN emmake make

# Misc.
# Even though this package fails to install,
# it leaves gnu/stubs-32.h in the include path.
# (libunistring header files are trying to include this 32 bit header).
RUN apt-get update && apt-get install -y libc6-dev-i386 || true