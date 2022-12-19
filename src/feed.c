#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct feed_t feed_t;
typedef struct line_t line_t;
typedef struct lines_t lines_t;
typedef struct token_t token_t;


struct token_t {
  uint8_t* ch;
  token_t* next;
  token_t* prev;
};

struct feed_t {
  token_t* start;
  token_t* end;
};

struct line_t {
  uint8_t* ch;
  line_t* next;
  line_t* prev;
};

struct lines_t {
  line_t* start;
};



void _feed_tokenize(feed_t* feed, uint8_t* str, size_t blen)
{
  feed->start = NULL;
  feed->end = NULL;

  // strtok works on null-terminated strings:
  str = realloc(str, blen + 1);
  str[blen] = '\0';

  uint8_t* ch = (uint8_t*) strtok((char*)str, "\n");
  token_t* token = (token_t*) malloc(sizeof(token_t));
  token->ch = ch;
  token->next = NULL;
  token->prev = NULL;

  feed->start = token;
  feed->end = token;

  token_t* cur = token;

  while (ch != NULL) {
    ch = (uint8_t*) strtok(NULL, "\n");
    if (ch == NULL) {
      break;
    }
    token_t* token = (token_t*) malloc(sizeof(token_t));
    token->ch = ch;
    token->next = NULL;
    token->prev = cur;

    cur->next = token;
    cur = token;

    feed->end = token;
  }
}

feed_t* feed_ctor(uint8_t* data, size_t blen) {
  feed_t* feed = malloc(sizeof(feed_t));
  _feed_tokenize(feed, data, blen);
  return feed;
}

lines_t* lines_ctor() {
  lines_t* lines = malloc(sizeof(lines_t));
  lines->start = NULL;
  return lines;
}

size_t feed_get_visible_lines(
    feed_t* feed,
    int height,
    int width,
    lines_t* lines
) {
  token_t* token = feed->end;

  line_t* line = (line_t*) malloc(sizeof(line_t));
  line->ch = token->ch; // token 2 line
  line->next = NULL;
  line->prev = NULL;
  
  line_t* cur = line;

  int i = 1;
  while ((token = token->prev) != NULL && i++ < height) {
  // while ((token = token->prev) != NULL) {
    line = (line_t*) malloc(sizeof(line_t));
    line->ch = token->ch; // token 2 line
    line->next = cur;

    cur = line;
  }

  lines->start = cur;
  return i;
}

uint8_t* lines_take(lines_t* lines)
{
  if (lines->start == NULL) {
    return NULL;
  }
  line_t* line = lines->start;
  lines->start = line->next;
  return line->ch;
}
