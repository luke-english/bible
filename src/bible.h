#pragma once

#define BLOG(fmt, ...) do { \
    fprintf(stderr, fmt " (%s:%d)\n", __VA_ARGS__, __FILE__, __LINE__); \
} while (0)