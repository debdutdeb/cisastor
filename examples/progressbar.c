#include "../libs/progressbar/progressbar.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define CHUNK_SIZE 1024
#define TOTAL_SIZE (1024 * 100) // 100KB

int main() {
  FILE *dest = fopen("output.bin", "wb");
  if (NULL == dest) {
    perror("fopen");
    return 1;
  }

  ProgressBarOptions opts = {
    .bytes_length = TOTAL_SIZE,
    .label        = "Downloading",
  };

  FILE *pb = progressbar_open(dest, &opts);
  if (NULL == pb) {
    perror("progressbar_open");
    fclose(dest);
    return 1;
  }

  char chunk[CHUNK_SIZE];
  memset(chunk, 0xAB, CHUNK_SIZE); // dummy data

  int total_written = 0;
  while (total_written < TOTAL_SIZE) {
    int to_write = CHUNK_SIZE;
    if (total_written + to_write > TOTAL_SIZE)
      to_write = TOTAL_SIZE - total_written;

    fwrite(chunk, 1, to_write, pb);
    total_written += to_write;

    usleep(10000); // 10ms delay so we can see the bar move
  }

  fclose(pb); // triggers __progressbar_close -> fclose(dest)

  fprintf(stderr, "Done! wrote output.bin\n");
  return 0;
}
