# History of this software

## History starts from now...

In November 2022 I started to work on the "second version" of this software.
The first version was somewhat working but it had some bugs and it was in an
unmaintaineable state. I started with a completelly new codebase and a new
repository.

This document describes bellow describes what was the past developmen of this
software, and the directionality of the file should be kept in a way where the
oldest events are described in the bottom and the latest will be in the top of
this file. As I am starting this document today, I will describe what what has
happened before now. Shoud some new events happen in the future they will be
added as new chapters above this one.

## The second version, and why first version will discontinue

This is and it will always be a terminal app. It is using curses library to
render the UI. Started as a CLI app it used ncurses implementation to build
linux CLI application binaries. The web UI came as an aftermath. I used
[emscripten](https://emscripten.org/) to compile the C code to WASM, but the
problem here is that curses is designed to run in terminal. Luckily there is
[emcurses](https://github.com/rhaberkorn/emcurses), a port of PDCurses. This
port uses [termlib.js](https://www.masswerk.at/termlib/), and the library
comes shipped together with termlib.js, and possibly is forked and outdated
also the way how I integrated it into my project was rather dirty. PDCurses
makes use *terminfo* library, but from what I can see it is already modified
to work with emscripten, the original PDCurses code is 
[patched](https://github.com/rhaberkorn/emcurses/commit/3cde95fb).

I started Bible-Overtype as a Linux CLI app, therefore relying on curses was a
natural choice. But I only use few functions out of it. There were chunks of
code just copied from somewhere just to make these things work together.

While *termlib* was a popular JS library to display terminal in a browser,
recently [Xterm.js](https://xtermjs.org/) became much more supported, and more
configurable. To migrate to Xterm.js would require replacing Emcurses. There
is a repository with [Ncurses + Xterm.js Emscripten build](
  https://github.com/jamesbiv/ncurses-emscripten). But I never managed to get
that working for me.

I realize my application only requres a small subset of curses. Which should
make it simple to re-implement curses.h in my own version of web-based curses.
The code I copied into my project is not well maintained, but that would mean
a major reinginering. And given the rest of my code is full of workarounds,
I decided to start over with this project, even got a new repository for it.
I will call the old version Bible-Overtype, and the new version will be just
Bible. It will make use of all what I learned to build the first version.

