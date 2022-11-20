#!/bin/bash

set -e
set -o pipefail

# clang-format \
#   --dry-run \
#   --style=Chromium \
#   /mnt/src/*.c /mnt/src/*.h

mkdir -p /tmp/obj

emcc /mnt/src/zcurses.c -c \
  -I /opt/emcurses \
  -o /tmp/obj/zcurses.bc.o

emcc /mnt/src/activity.c -c \
  -I /opt/emcurses \
  -o /tmp/obj/activity.bc.o

emcc /mnt/src/ctx.c -c \
  -o /tmp/obj/ctx.bc.o

emcc /mnt/src/program.c -c \
  -I /opt/emcurses \
  -o /tmp/obj/program.bc.o

emcc /mnt/src/scripture.c -c \
  -o /tmp/obj/scripture.bc.o

emcc \
  -O3 \
  -s WASM=1 \
  --minify 0 \
  --no-entry \
  -I /opt/emcurses \
  /mnt/src/bible.c \
  /tmp/obj/activity.bc.o \
  /tmp/obj/ctx.bc.o \
  /tmp/obj/program.bc.o \
  /tmp/obj/scripture.bc.o \
  /tmp/obj/zcurses.bc.o \
  /opt/emcurses/emscripten/libpdcurses.so \
  -o /mnt/wasm/bible.wasm

