#!/bin/bash

set -e
set -o pipefail

mkdir -p /tmp/obj

gcc /mnt/src/zcurses.c -c \
  -o /tmp/obj/zcurses.o

gcc /mnt/src/activity.c -c \
  -o /tmp/obj/activity.o

gcc /mnt/src/ctx.c -c \
  -o /tmp/obj/ctx.o

gcc /mnt/src/program.c -c \
  -o /tmp/obj/program.o

gcc /mnt/src/scripture.c -c \
  -o /tmp/obj/scripture.o
#
#
gcc \
  -O3 \
  /mnt/src/bible.c \
  /tmp/obj/activity.o \
  /tmp/obj/ctx.o \
  /tmp/obj/program.o \
  /tmp/obj/scripture.o \
  /tmp/obj/zcurses.o \
  -o /mnt/exec/bible \
  -lncurses


