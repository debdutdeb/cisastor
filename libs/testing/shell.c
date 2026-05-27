#include <stdint.h>
#include <stdio.h>

uint8_t run_command(const char *const args) {
  FILE *pipe = popen(args, "r");
  if (!pipe) {
    perror("pipe");
    return 1;
  }
  for (int c = fgetc(pipe); c != EOF; c = fgetc(pipe))
    putchar(c);
  return 0;
}
