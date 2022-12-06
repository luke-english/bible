#pragma once

#include "zcurses.h"

typedef void global_cb_t(int ch);

void ingest_ch(int key);
void wgetch_async(WINDOW* win, global_cb_t* cb);