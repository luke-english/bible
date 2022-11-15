#!/bin/bash

set -e
set -o pipefail

# clang-format \
#   --dry-run \
#   --style=Chromium \
#   /mnt/src/*.c /mnt/src/*.h
#
mkdir -p /tmp/obj

emcc /mnt/src/zcurses.c -c \
  -o /tmp/obj/zcurses.bc.o

emcc /mnt/src/activity.c -c \
  -o /tmp/obj/activity.bc.o

emcc /mnt/src/ctx.c -c \
  -o /tmp/obj/ctx.bc.o

emcc /mnt/src/program.c -c \
  -o /tmp/obj/program.bc.o

emcc /mnt/src/scripture.c -c \
  -o /tmp/obj/scripture.bc.o
#
#
emcc \
  -O3 \
  -s WASM=1 \
  --no-entry \
  /mnt/src/bible.c \
  /tmp/obj/activity.bc.o \
  /tmp/obj/ctx.bc.o \
  /tmp/obj/program.bc.o \
  /tmp/obj/scripture.bc.o \
  /tmp/obj/zcurses.bc.o \
  -o /mnt/wasm/bible.wasm


