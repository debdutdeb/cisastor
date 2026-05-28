# Arena Allocator

The Arena Allocator provides a simple way to manage memory by allocating it in large blocks (regions). This is particularly useful for tasks where many small objects are allocated and can all be freed at once at the end of the program.

## Usage

Include the header:
```c
#include "arena.h"
```

### Basic Allocation
You can allocate memory using `aalloc` (an alias for `arena_alloc`). The allocator handles the underlying memory management, and all memory is automatically cleaned up when your program exits.

```c
// Allocate space for a single integer
int *my_int = aalloc(sizeof(int));
*my_int = 42;

// Allocate an array of 10 floats
float *my_floats = aalloc(sizeof(float) * 10);
my_floats[0] = 3.14f;
```

### String Utilities
The arena provides a helper for creating empty strings that are automatically managed by the arena.

```c
// Create an empty string buffer for 100 characters (+1 for null terminator)
char *buffer = string_create_empty(100);
strcpy(buffer, "Hello, Arena!");
```

## API Reference

| Function / Macro | Description |
| --- | --- |
| `void *arena_alloc(size_t size)` | Allocates `size` bytes. Returns a pointer to the allocated memory. |
| `aalloc(size)` | Macro alias for `arena_alloc`. |
| `char *string_create_empty(size_t characters)` | Allocates and null-terminates a string buffer of given size. |
| `reaalloc(ptr, size)` | Currently behaves as `aalloc(size)`. Useful for future-proofing code that may need reallocation. |
