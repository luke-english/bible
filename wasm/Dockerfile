FROM emscripten/emsdk

WORKDIR /opt
RUN git clone --branch 10x-curses https://github.com/luke10x/emcurses.git
WORKDIR /opt/emcurses/emscripten
RUN emmake make
