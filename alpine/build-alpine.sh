#!/bin/bash

set -e
set -o pipefail

mkdir -p /tmp/obj

gcc /mnt/src/feed.c -c \
  -o /tmp/obj/feed.o -lunistring

gcc /mnt/src/activity.c -c \
  -o /tmp/obj/activity.o

gcc /mnt/src/ctx.c -c \
  -o /tmp/obj/ctx.o

gcc /mnt/src/program.c -c \
  -o /tmp/obj/program.o

gcc /mnt/src/scripture.c -c \
  -o /tmp/obj/scripture.o


gcc \
  /mnt/src/test.c \
  -o /tmp/test \
  -lunistring

/tmp/test

echo "💚 test done"

gcc \
  -O3 \
  /mnt/src/bible.c \
  /tmp/obj/feed.o \
  /tmp/obj/activity.o \
  /tmp/obj/ctx.o \
  /tmp/obj/program.o \
  /tmp/obj/scripture.o \
  -o /tmp/bible \
  -lncurses \
  -lunistring


