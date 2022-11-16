#include <stdlib.h>

#include "ctx.h"

typedef struct ctx_t ctx_t;

struct ctx_t {
  int rows;
  int cols;
};

int ctx_get_rows(ctx_t* ctx) {
  return ctx->rows;
}

int ctx_get_cols(ctx_t* ctx) {
  return ctx->cols;
}

void ctx_set_size(ctx_t* ctx, int rows, int cols) {
  ctx->rows = rows;
  ctx->cols = cols;
}

