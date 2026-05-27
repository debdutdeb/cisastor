/*
 * struct testing is a pretty object to control runtime of many tests.
 * It is a stateless object, therefore is not guaranteed any behavior.
 */

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "testing.h"

void tfailf(testing *t, char *fmt, ...) {
  t->is_failing = 1;
  fprintf(stderr, "%s: FAILED\n    ", t->tag);
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
}

void tfail(testing *t, char *message) {
  tfailf(t, message);
  fprintf(stderr, "\n");
}

void tskipf(testing *t, char *fmt, ...) {
  t->is_skipped = 1;
  if (fmt == NULL) {
    fprintf(stderr, "%s: SKIPPED\n", t->tag);
    return;
  }
  fprintf(stderr, "%s: SKIPPED\n    ", t->tag);
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
}

void tskip(testing *t, char *message) {
  tskipf(t, message);
  fprintf(stderr, "\n");
}
