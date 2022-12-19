#!/bin/bash

set -e
set -o pipefail

# clang-format \
#   --dry-run \
#   --style=Chromium \
#   /mnt/src/*.c /mnt/src/*.h

mkdir -p /tmp/obj

emcc /mnt/src/zcurses.c -c \
  -I /opt/pdcurses \
  -I /opt/pdcurses/wasm \
  -o /tmp/obj/zcurses.bc.o

emcc /mnt/src/getch_async.c -c \
  -I /opt/pdcurses \
  -I /opt/pdcurses/wasm \
  -o /tmp/obj/getch_async.bc.o

emcc /mnt/src/feed.c -c \
  -I /opt/pdcurses \
  -I /opt/pdcurses/wasm \
  -o /tmp/obj/feed.bc.o

emcc /mnt/src/activity.c -c \
  -I /opt/pdcurses \
  -I /opt/pdcurses/wasm \
  -o /tmp/obj/activity.bc.o

emcc /mnt/src/ctx.c -c \
  -o /tmp/obj/ctx.bc.o

emcc /mnt/src/program.c -c \
  -I /opt/emcurses \
  -I /opt/pdcurses/wasm \
  -o /tmp/obj/program.bc.o

emcc /mnt/src/scripture.c -c \
  -o /tmp/obj/scripture.bc.o


emcc \
  -O3 \
  -s WASM=1 \
  -s ASYNCIFY \
  -s ASYNCIFY_IMPORTS=[await_timeout] \
  --minify 0 \
  --no-entry \
  -I /opt/pdcurses \
  -I /opt/pdcurses/wasm \
  /mnt/src/bible.c \
  /tmp/obj/feed.bc.o \
  /tmp/obj/activity.bc.o \
  /tmp/obj/ctx.bc.o \
  /tmp/obj/program.bc.o \
  /tmp/obj/scripture.bc.o \
  /tmp/obj/zcurses.bc.o \
  /tmp/obj/getch_async.bc.o \
  /opt/pdcurses/wasm/libpdcurses.so \
  -o /mnt/wasm/bible.wasm

