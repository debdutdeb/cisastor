# How to Write and Run Tests

Cisastor provides a simple, macro-based testing framework for C. It supports automated test discovery and a flexible test runner.

## Writing Tests

To write a test, create a C file with the suffix `_test.c` (e.g., `my_feature_test.c`).

### Basic Structure

```c
#include "testing.h"

// Define a test using the TEST macro
TEST(should_perform_expected_behavior) {
    int expected = 42;
    int actual = 40 + 2;

    if (actual != expected) {
        failf("Expected %d, but got %d", expected, actual);
    }
}
```

### Assertions and Control

The framework provides the following macros for test control:

- `fail(message)`: Marks the test as failed with the given message and stops execution of the current test.
- `failf(format, ...)`: Marks the test as failed with a formatted message and stops execution.
- `skip`: Skips the current test.
- `skip_because(message)`: Skips the current test with a reason.
- `skip_becausef(format, ...)`: Skips the current test with a formatted reason.

### Test Discovery

The framework uses `__attribute__((constructor))` to register tests automatically. You don't need to manually add your tests to a suite; just defining them with `TEST()` is enough.

---

## Running Tests

There are two primary ways to run tests: using the discovery runner or manual compilation.

### 1. Using the Discovery Runner

If you have the `testing` binary built, you can run it in any directory. It will:
1. Scan for all `*_test.c` files.
2. Compile them automatically.
3. Link them into a temporary executable.
4. Run all discovered tests.

**Example:**
```bash
./path/to/testing
```

**Debug Mode:**
To see the commands being executed during discovery and compilation, set the `DEBUG` environment variable:
```bash
DEBUG=true ./path/to/testing
```

### 2. Manual Compilation

You can also compile your tests manually by linking against the testing library and its entry point.

```bash
cc -c my_test.c -o my_test.o
cc my_test.o libs/testing/testing.o -Llibs/testing -ltesting -o my_test_runner
./my_test_runner
```

### Test Runner Options

The resulting test binary (either the discovery runner or a manually compiled one) supports the following flags:

- `-l`: List all registered tests.
- `-n <pattern>`: List tests matching the given pattern.
- `-t <pattern>`: Run only tests matching the given pattern.
- (no flags): Run all tests.

---

## Integration with CMake

The project provides a CMake helper function `cisastor_add_test` defined in `libs/testing/CisastorTesting.cmake`.

```cmake
include(libs/testing/CisastorTesting.cmake)

cisastor_add_test(
    NAME my_feature_test
    SRCS my_feature_test.c
    LIBS my_library
)
```

This will create a test executable and register it with CTest. You can then run tests using:
```bash
ctest
```

## Advanced Configuration

The framework can be configured using preprocessor definitions:

- `CISASTOR_TESTING_HALT_ON_FAIL`: If defined as `1`, the test runner will stop immediately after the first test failure. Default is `0`.
- `MAX_TESTS`: The maximum number of tests that can be registered. Default is `1024`.
- `MAX_TEST_FILES`: The maximum number of test files the discovery runner can process. Default is `1000`.

Example of setting a configuration in your test file:
```c
#define CISASTOR_TESTING_HALT_ON_FAIL 1
#include "testing.h"
```
