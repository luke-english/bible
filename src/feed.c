#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistr.h>
#include <uniwidth.h>
#include <uniwbrk.h>
#include <unilbrk.h>
#include <uninorm.h>

typedef struct feed_t feed_t;
typedef struct line_t line_t;
typedef struct lines_t lines_t;
typedef struct token_t token_t;

/*
The process is divided into three parts:

1. TOKENIZATION

Tokens are lines from entire original file.
lines are constructed out of tokens
it only needs to construct as many lines is needed to fill the screen
it loops from the last token backwards to take only as many
tokens as needed to build required lines.

2. SELECTION

it stops looping the tokens back when it reaches number of characters
required to fill the screen full.
(Unless "biblical mode is enabled",
then it will keep going until previous token is properly terminated)

3. WRAPPING

Then it splits the tokes into lines.
if it has more lines than the height of the screen
(which is quite possible due to long tokens, and words wrapped),
then it only takes as many lines as required from the bottom of the screen
*/

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
  line_t* head;
  line_t* current;
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


int _lines_split_string(lines_t* lines, uint8_t *string, int width) {

  size_t start = 0;
  size_t end = 0;
  size_t length = u8_strlen(string);

  int count = 0;
  char* breaks = malloc(sizeof(char) * strlen(string));


  int last = u8_width_linebreaks (
    string, strlen(string), 100, 0, 0, NULL, "", breaks);


  breaks = malloc(sizeof(char) * strlen(string));
  u8_wordbreaks (string, length, breaks);

  char* msg;
  int i = 0;
  int w = 0;
  while (start < length) {
    end = width;
   
    for (i = start+1, w = 0; i < length; i++) {
      int x = u8_mblen(string+i, strlen((char*)(string+i)));
      w += ((x > 0) ? x : 0);
      
      if (w >= width) {
        // end = i;
        break;
      }

      bool can_break = (breaks[i] == 1) || (string[i] == ' ');
      if (string[i] == ',' 
        || string[i] == '.'
        || string[i] == '?'
        || '!' == string[i]
        || ':' == string[i]
        || string[i] == ';'
        || string[i] == '\''
        || string[i] == '\"'
        || string[i] == '(' 
        || string[i] == ')' 
        || string[i] == '[' 
        || string[i] == ']' 
        || string[i] == '{' 
        || string[i] == '}' 
        || string[i] == '<' 
        || string[i] == '>' 
        || string[i] == ',' 
        || string[i] == '.' 
        || string[i] == '?' 
        || string[i] == ';' 
        || string[i] == ':' 
        || string[i] == ';' 
        || string[i] == '!' 
        || string[i] == '?' 
        || string[i] == ')' 
        || string[i] == '(' 
        || string[i] == '[' 
        || string[i] == ']' 
        || string[i] == '{' 
        || string[i] == '}' 
        || string[i] == '<' 
        || string[i] == '>') {
        can_break = false;
      }
      if (can_break) {
        end = i - start;  
      }
    }
    // Allocate a new node for the split string
    line_t* node = malloc(sizeof(line_t));
    node->ch = strndup(string + start, end);
    node->next = NULL;
    node->prev = NULL;

msg = malloc(sizeof(char) * 1000);
sprintf(msg, "[%d/%d] %s", end, width, node->ch);
node->ch = msg;

    node->next = NULL;

    // Add the node to the linked list
    if (lines->head == NULL) {
      lines->head = node;
      lines->current = node;
    } else {
      lines->current->next = node;
      lines->current = lines->current->next;
      lines->current = node;
    }
    start += end;
    count++;
  }

  return count;
}

feed_t* feed_ctor(uint8_t* data, size_t blen) {
  feed_t* feed = malloc(sizeof(feed_t));
  _feed_tokenize(feed, data, blen);
  return feed;
}

lines_t* lines_ctor() {
  lines_t* lines = malloc(sizeof(lines_t));
  lines->head = NULL;
  lines->current = NULL;
  return lines;
}

/*
 * Returns a linked list of lines of current screen.
 */
size_t feed_get_visible_lines(
    feed_t* feed,
    int height,
    int width,
    lines_t* lines
) {
  // Selection
  uint32_t area = height * width;
  uint32_t selected_area = 0;
  token_t* curtok = feed->end;

  while (selected_area < area) {
    selected_area += u8_width(curtok->ch, strlen(curtok->ch), "UTF-8");
    if (curtok->prev == NULL) {
      break;
    }
    curtok = curtok->prev;
  }

  // Wrapping 
  int total = 0;
  while (curtok != NULL) {
    total += _lines_split_string(lines, curtok->ch, 14);
    curtok = curtok->next;
  }

  lines->current = lines->head;

  while (total-- > height) {
    lines->current = lines->current->next;
  }

  int i = 1;
  return i;
}

uint8_t* lines_take(lines_t* lines)
{
  if (lines->current == NULL) {
    return NULL;
  }
  line_t* line = lines->current;
  lines->current = line->next;
  return line->ch;
}
