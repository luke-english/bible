FROM emscripten/emsdk:3.1.25
WORKDIR /opt

# Take patched distribution of PDCurses
RUN git clone --branch wasm https://github.com/luke10x/PDCurses.git pdcurses
ENV PDCURSES_SRCDIR=/opt/pdcurses

# WASM-platform is stored in this repo for now
ADD pdcurses/wasm /opt/pdcurses/wasm

WORKDIR /opt/pdcurses/wasm

RUN emmake make
