#pragma once

#include <limits.h>
#include <stdint.h>

/*
 * Testing helper lib for cisastor set of libraries.
 * Adds helpers to define expected and wanted test cases.
 * Adds helpers to instrument running of each tests.
 * Only supports sequential testing, for now.
 */

#ifndef MAX_TESTS
#define MAX_TESTS 1024
#endif

#ifndef MAX_TEST_FILES
#define MAX_TEST_FILES 1000
#endif

#ifndef CISASTOR_TESTING_HALT_ON_FAIL
#define CISASTOR_TESTING_HALT_ON_FAIL 0
#endif

#ifndef CISASTOR_TESTING_RUN_CONCURRENTLY
#define CISASTOR_TESTING_RUN_CONCURRENTLY 0
#else
#error cisastor testing currently does not support concurrent runtime
#endif

typedef struct {
  const char *tag;
  uint8_t is_failing;
  uint8_t is_skipped;
} testing;

typedef void (*test_runner)(testing *);

typedef struct {
  char *tag;
  test_runner runner;
} test;

// a hard limit of maximum tests cisastor can accumulate, but should be enough
extern test tests[MAX_TESTS];

extern int test_index;

/*
 * Usage:
 * void add_x_y(testing* t) {
 *     T("should add x and y", add_x_y);
 * }
 */
#define T(label, fn)                                                           \
  do {                                                                         \
    test _t = {.tag = label, .runner = fn};                                    \
                                                                               \
    tests[test_index++] = _t;                                                  \
                                                                               \
  } while (0)

// TEST(add_x_y) {
//  // run test with t in scope;
// }
#define TEST(name)                                                             \
  static void name(testing *t);                                                \
  __attribute__((constructor)) static void register_##name(void) {             \
    T(#name, name);                                                            \
  }                                                                            \
  static void name(testing *t)

#define IT TEST

void tfail(testing *t, char *message);
void tfailf(testing *t, char *fmt, ...);
void tskip(testing *t, char *message);
void tskipf(testing *t, char *fmt, ...);

#define fail(message)                                                          \
  do {                                                                         \
    tfail(t, message);                                                         \
    return;                                                                    \
  } while (0)
#define failf(fmt, ...)                                                        \
  do {                                                                         \
    tfailf(t, fmt, __VA_ARGS__);                                               \
    return;                                                                    \
  } while (0)

#define skip_because(message)                                                  \
  do {                                                                         \
    tskip(t, message);                                                         \
    return;                                                                    \
  } while (0)
#define skip_beausef(fmt, ...)                                                 \
  do {                                                                         \
    tskipf(t, fmt, __VA_ARGS__);                                               \
    return;                                                                    \
  } while (0)
#define skip                                                                   \
  do {                                                                         \
    tskip(t, NULL);                                                            \
    return;                                                                    \
  } while (0)

#define cassertf(cond, fmt, ...)                                               \
  do {                                                                         \
    if (!(cond))                                                               \
      failf(fmt, __VA_ARGS__);                                                 \
  } while (0)

#define cassert(cond, message)                                                 \
  do {                                                                         \
    if (!(cond))                                                               \
      fail(message);                                                          \
  } while (0)
