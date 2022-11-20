# Bible

Study Bible while practicing Liturgical Typing. Follow the Word of God marking
this journey with a print of the letters as you type them. This repository
contains the source code of command line application **Bible**, and the Web
application allowing to run it in a browser.

## Architecture

TBA

## Quickstart

Build WASM binary:

    docker-compose run wasm

Build JS bundle:

    docker-compose run node

    # To keep it building:
    docker-compose run node 'npm run watch'

Start local Web server publishing the WASM application
in a browser:

    docker-compose run web

Start as a CLI application in a Docker container.

    docker-compose run alpine
