# Testing Framework

A simple unit testing framework for Cisastor.

## API

### Defining Tests

#### `TEST(name)` / `IT(name)`
Defines a test function. Tests are automatically registered.

### Assertions

#### `cassert(cond, message)`
Asserts that `cond` is true, otherwise fails the test with `message`.

#### `cassertf(cond, fmt, ...)`
Formatted version of `cassert`.

#### `fail(message)` / `failf(fmt, ...)`
Manually fails a test.

### Utilities

#### `uint8_t strcmp_at_any(const char *const src, const char *const match)`
Checks if `match` is a substring of `src`.

#### `uint8_t run_command(const char *const args)`
Executes a shell command and prints output. Returns 0 on success.
