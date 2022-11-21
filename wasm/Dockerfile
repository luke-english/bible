FROM emscripten/emsdk

WORKDIR /opt

RUN git clone --branch wasm https://github.com/luke10x/PDCurses.git pdcurses
WORKDIR /opt/pdcurses/wasm

RUN emmake make
