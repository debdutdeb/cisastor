/*
 * vi: set tabstop=2
 * ProgressBar implements the same architecture of gopark;
 * A ProgressBar is a drop in replacement of std::ostream;
 */

#define _GNU_SOURCE

#include "macros.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <sys/ioctl.h>
#include <unistd.h>

typedef struct progress_bar_options {
  int bytes_length;
  char *label;
} ProgressBarOptions;

typedef struct progress_bar {
  ProgressBarOptions *options;

  FILE* dest;

  int written;
  float percent;
  int columns_written;
  int bar_width;

  char *bar;
} ProgressBar;

const char *extra_characters = "[] [] xxx.xx%";
const int extra_character_length = 13 + 1 /* \0 */;

char *progressbar_build(struct progress_bar *const pb, int n) {
  pb->written += n;

  pb->percent = (float)pb->written / pb->options->bytes_length * 100.00;

  int fill = floor(floor(pb->bar_width) * pb->percent / 100.00);

  for (int i = pb->columns_written; i < fill; i++) {
	  pb->bar[i] = '=';
  }

  pb->columns_written = fill;

  return pb->bar;
}

static long __progressbar_write(void *cookie, const char *buf, unsigned long n) {
  ProgressBar *pb = cast(ProgressBar *, cookie);

  if (0 == n) {
	  return fwrite(buf, 1, 0, pb->dest);
  }

  fprintf(stderr, "\r[%s] [%s] %.2f%%", pb->options->label, progressbar_build(pb, n), pb->percent);
  fflush(stderr);

  return fwrite(buf, 1, n, pb->dest);
}

static int __progressbar_close(void *cookie) {
    ProgressBar *pb = cast(ProgressBar *, cookie);
    fprintf(stderr, "\n");
    fflush(stderr);
    int ret = fclose(pb->dest);
    free(pb->bar);
    free(pb);
    return ret;
}

FILE *progressbar_open(FILE *fp, ProgressBarOptions *options) {
  if (null == options || null == options->label) {
    return null;
  }

  ProgressBar *pb = cast(ProgressBar *, calloc(1,sizeof(struct progress_bar)));
  if (null == pb) {
    errno = ENOMEM;
    return null;
  }

  pb->options = options;

  struct winsize win;
  if (-1 == ioctl(STDERR_FILENO, TIOCGWINSZ, &win)) {
	  win.ws_col = 80; // fallback
  }

  pb->bar_width = (int)win.ws_col - (int)strlen(options->label) - extra_character_length;
  if (pb->bar_width < 1) {
	  pb->bar_width = 1;
  }

  pb->bar = cast(char*, malloc(sizeof(char)*pb->bar_width + 1));
  if (null == pb->bar) {
	  free(pb);
	  return null;
  }

  memset(pb->bar, ' ', pb->bar_width);
  pb->bar[pb->bar_width] = 0;

  pb->dest = fp;

#ifdef __APPLE__
    return funopen(pb, NULL, __progressbar_write, NULL, __progressbar_close);
#else
    cookie_io_functions_t io = { .write = __progressbar_write, .close = __progressbar_close };
    return fopencookie(pb, "w", io);
#endif
}
