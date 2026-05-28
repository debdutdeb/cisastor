# Testing Framework

Cisastor includes a minimal, built-in unit testing framework. It allows you to define tests that are automatically registered and executed.

## Usage

### Writing a Test
Tests are defined using the `TEST` (or `IT`) macro.

```c
#include "testing.h"

TEST(should_add_numbers) {
    int result = 2 + 2;
    
    // Simple assertion
    cassert(result == 4, "Addition failed!");
}

IT(should_format_messages) {
    int val = 5;
    // Assertion with printf-style formatting
    cassertf(val > 10, "Expected > 10, but got %d", val);
}
```

### Manual Failure and Skipping
```c
TEST(some_edge_case) {
    if (critical_error) {
        fail("Cannot proceed with test");
    }
    
    if (not_supported_on_platform) {
        skip_because("Platform not supported");
    }
}
```

## API Reference

### Macros

| Macro | Description |
| --- | --- |
| `TEST(name)` / `IT(name)` | Defines a test case. |
| `cassert(cond, msg)` | Fails the test if `cond` is false, printing `msg`. |
| `cassertf(cond, fmt, ...)` | Fails the test if `cond` is false, with a formatted message. |
| `fail(msg)` | Immediately fails the test. |
| `failf(fmt, ...)` | Immediately fails the test with a formatted message. |
| `skip_because(msg)` | Skips the remainder of the test. |

### Utilities

#### `uint8_t strcmp_at_any(const char *src, const char *match)`
Returns 1 if `match` exists anywhere inside `src`. Useful for checking substrings in test output.

#### `uint8_t run_command(const char *args)`
Executes a shell command and prints its output to the terminal. Returns 0 on success.
