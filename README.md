# Bible

Study Bible while practicing Liturgical Typing. Follow the Word of God marking
this journey with a print of the letters as you type them. This repository
contains the source code of command line application **Bible**, and the Web
application allowing to run it in a browser.

## Technology

This application is unique because it uses the same codebase to compile it to
two different products:

- A **Web application**, is built with [Emscripten compiler][1] and it shipped
  as a [WebAssembly container][2], and can be ran in a browser.
- A classic **CLI application**, built with standard `gcc`, it can run natively
  on any UNIX-like environment.

This is a text-mode UI application written in C language, based on
[Curses library][3]. However, the Web application uses custom Curses
implementation, while CLI application uses standard NCurses.

The Web application uses a patched version of [PDCurses][4]. This patched
version is inspired by [Emcurses project][5], just for this application
[Xterm.js][6] is used as a terminal emulator which enables redering TUI
applications in a browser (Emcurses uses Termlib, which is outdated).

## Quickstart

To start as a **CLI application** in a Docker container is very simple:

    docker-compose run alpine

To start a **Web application** requires a bit more steps:

Build WASM binary:

    docker-compose run wasm

Build JS bundle:

    # Only required on first run
    docker-compose run node 'npm ci'

    docker-compose run node

    # To keep it building:
    docker-compose run node 'npm run watch'

Start local Web server publishing the WASM application
in a browser:

    docker-compose run web

[1]: https://emscripten.org/    
[2]: https://webassembly.org/
[3]: https://en.wikipedia.org/wiki/Curses_(programming_library)
[4]: https://pdcurses.org/
[5]: https://github.com/rhaberkorn/emcurses
[6]: https://xtermjs.org/
