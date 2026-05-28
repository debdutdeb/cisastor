#include "testing.h"
#include "functions.h"
#include "shell.h"

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef CISASTOR_LIBTESTING_PATH
#define CISASTOR_LIBTESTING_PATH ""
#endif

#ifndef CISASTOR_TESTING_ENTRY_OBJ
#define CISASTOR_TESTING_ENTRY_OBJ ""
#endif

test tests[MAX_TESTS] = {};
int test_index = 0;

uint8_t _t = 1;
uint8_t _f = 0;

uint8_t *is_debug = NULL;

char *copy_string(char *str) {
  size_t n = strlen(str) + 1;
  char *cpy = malloc(sizeof(char) * n);
  assert(cpy != NULL && "failed to allocate buffer for copy string");
  cpy[n - 1] = 0;
  memcpy(cpy, str, n - 1);
  return cpy;
}

void debugf(const char *const fmt, ...) {
  if (is_debug == NULL) {
    char *debug = getenv("DEBUG");
    if (debug == NULL) {
      is_debug = &_f;
    } else if (strlen(debug) != 4) {
      is_debug = &_f;
    } else {
      char *true_string = "true";
      for (int i = 0; i < 4; i++) {
        if (true_string[i] != tolower(debug[i])) {
          is_debug = &_f;
          return;
        }
      }
      is_debug = &_t;
    }
  }
  if (!*is_debug) {
    return;
  }
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
}

#define TEST_FILE_SUFFIX "_test.c"
#define TEST_FILE_SUFFIX_LEN 7

uint8_t compile_and_rerun(char **argv, char **envp) {
  char *files[MAX_TEST_FILES] = {};
  DIR *dir = opendir("./");
  if (dir == NULL) {
    perror("opendir");
    return 1;
  }

  struct dirent *entry;

  int i = 0;
  while ((entry = readdir(dir)) != NULL) {
    size_t len = strlen(entry->d_name);
    for (int j = TEST_FILE_SUFFIX_LEN - 1, k = len - 1; j >= 0; j--, k--) {
      if (TEST_FILE_SUFFIX[j] != entry->d_name[k]) {
        goto next_entry;
      }
    }
    files[i++] = copy_string(entry->d_name);
    debugf("debug: found test file %s\n", files[i - 1]);
  next_entry:
    (void)1;
  }

  closedir(dir);

  // cc linked to final compiler
  // TODO: env var to use a compiler
  for (int j = 0; j < i; j++) {
    char *cmd_fmt = "cc -g -c %s -o %s";
    size_t n = strlen(files[j]) + 1;
    char *out = malloc(sizeof(char) * n);
    assert(out != NULL && "failed to allocate buffer for compiling test files");
    memcpy(out, files[j], n - 1);
    out[n - 2] = 'o';
    out[n - 1] = 0;
    n = strlen(cmd_fmt) + (n * 2) + 1;
    char *cmd = malloc(sizeof(char) * n);
    assert(cmd != NULL &&
           "failed to allocate command buffer for compiling test files");
    cmd[n] = 0;
    sprintf(cmd, cmd_fmt, files[j], out);
    debugf("debug: compiling %s to %s\n", files[j], out);
    debugf("debug:     running command %s\n", cmd);
    if (run_command(cmd) == 1) {
      fprintf(stderr, "failed to compile %s\n", files[j]);
      return 1;
    }
    free(files[j]);
    files[j] = out;
  }

  files[i++] = CISASTOR_TESTING_ENTRY_OBJ;

  if (i == 0) {
    fprintf(stderr, "no test files found\n");
    return 0;
  }

  char c_testing_out[L_tmpnam];
  memset(c_testing_out, 0, L_tmpnam);
  assert(tmpnam(c_testing_out) != NULL &&
         "failed to generate name for final executable");

  char *file_list_fmt = "%s %s";
  size_t n = 1; // space
  char *file_list_str = "";
  for (int j = 0; j < i; j++) {
    // TODO: using a list for run_command would be much simpler
    n = strlen(files[j]) + n + strlen(file_list_str) + 1;
    char *s = malloc(sizeof(char) * n);
    assert(s != NULL && "failed to allocate buffer for command full file list");
    s[n - 1] = 0;
    sprintf(s, file_list_fmt, file_list_str, files[j]);
    file_list_str = s;
  }

  debugf("debug: file list: %s\n", file_list_str);

  char *cmd_fmt = "sh -c \"cc %s -o %s -L$(dirname %s) -ltesting\"";
  n = strlen(cmd_fmt) - 6 /* format modifiers */ + strlen(file_list_str) +
      strlen(c_testing_out) + strlen(CISASTOR_LIBTESTING_PATH) + 1;
  char *cmd = malloc(sizeof(char) * n);
  assert(cmd != NULL && "failed to allocate buffer for command gen");
  cmd[n - 1] = 0;
  sprintf(cmd, cmd_fmt, file_list_str, c_testing_out, CISASTOR_LIBTESTING_PATH);
  debugf("debug: out gen command: \n    %s\n", cmd);

  if (run_command(cmd) == 1) {
    fprintf(stderr, "failed to generate final executable for tests\n");
    return 1;
  }

  argv[0] = c_testing_out;

  debugf("debug: running \"%s\"\n", c_testing_out);
  execve(c_testing_out, argv, envp);
  perror("execve");
  return 1;
}

#define BINARY_NAME "testing"
#define BINARY_NAME_LEN 7

uint8_t is_own_binary(const char *const name) {
  size_t len = strlen(name);
  if (len < BINARY_NAME_LEN) {
    return 0;
  }
  for (int i = len - 1, j = BINARY_NAME_LEN - 1; j >= 0; i--, j--) {
    if (BINARY_NAME[j] != name[i]) {
      return 0;
    }
  }
  return 1;
}

struct summary {
  uint64_t failed;
  uint64_t succeeded;
  uint64_t total;
};

uint8_t print_summary_and_return(struct summary *summary) {
  fprintf(stderr, "PASSED: %lld\nFAILED: %lld\nSKIPPED: %lld\n",
          summary->succeeded, summary->failed,
          summary->total - summary->succeeded - summary->failed);
  return summary->failed > 0 ? 1 : 0;
}

uint8_t run_test(const char *const tag) {
  // Find all the test files first,
  // Build the new executable then exec into it
  struct summary summary = {.failed = 0, .succeeded = 0, .total = test_index};
  for (int i = 0; i < test_index; i++) {
    test this = tests[i];
    testing t = {.tag = this.tag, .is_failing = 0, .is_skipped = 0};
    if (tag != NULL) {
      if (!strcmp_at_any(this.tag, tag)) {
        tskipf(&t, "matcher tag \"%s\" did not match \"%s\"\n", tag, this.tag);
      } else {
        this.runner(&t);
      }
    } else {
      this.runner(&t);
    }
    if (t.is_failing) {
      summary.failed++;
      if (CISASTOR_TESTING_HALT_ON_FAIL) {
        return print_summary_and_return(&summary);
      }
    } else if (!t.is_skipped) {
      printf("%s: PASSED\n", this.tag);
      summary.succeeded++;
    }
  }
  return print_summary_and_return(&summary);
}

void list_tests() {
  for (int i = 0; i < test_index; i++) {
    fprintf(stderr, "%s\n", tests[i].tag);
  }
  fprintf(stderr, "Total tests: %d\n", test_index);
}

void find_tests(const char *const tag) {
  int count = 0;
  for (int i = 0; i < test_index; i++) {
    if (strcmp_at_any(tests[i].tag, tag)) {
      fprintf(stderr, "%s\n", tests[i].tag);
      count++;
    }
  }
  fprintf(stderr, "Total tests found: %d\n", count);
}

int main(int argc, char **argv, char **envp) {
  if (is_own_binary(argv[0])) {
    return compile_and_rerun(argv, envp);
  }
  int opt;
  while ((opt = getopt(argc, argv, "lt:n:")) != -1) {
    switch (opt) {
    case 'n':
      find_tests(optarg);
      return 0;
    case 'l':
      list_tests();
      return 0;
    case 't':
      return run_test(optarg);
    default:
      return run_test(0);
    }
  }
  return run_test(0);
}
