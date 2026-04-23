#pragma once
#include <stdio.h>

typedef struct progress_bar_options {
  int   bytes_length;
  char *label;
} ProgressBarOptions;

FILE *progressbar_open(FILE *fp, ProgressBarOptions *options);
