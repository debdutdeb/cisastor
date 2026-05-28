# Arena Allocator

A simple region-based memory allocator that manages memory in chunks.

## API

### Functions

#### `void *arena_alloc(size_t size)`
Allocates `size` bytes from the current arena chunk. If the current chunk is full or uninitialized, a new chunk is allocated. Memory is automatically freed when the program exits via `atexit`.

#### `char *string_create_empty(size_t characters)`
Allocates a null-terminated string of the given character length in the arena.

### Macros

#### `aalloc(size)`
Alias for `arena_alloc(size)`.

#### `reaalloc(a, b)`
Alias for `aalloc(b)`. Note: Currently does not reallocate existing memory, just allocates a new block.
