# Utils

The `utils` library contains foundational macros and type definitions used across the Cisastor collection.

## API Reference

Include the header:
```c
#include "macros.h"
```

### Macros

#### `cast(type, value)`
A standard way to perform explicit casts in the codebase.
- **Example**: `int *p = cast(int *, malloc(sizeof(int)));`

#### `null`
A lowercase alias for `NULL`.
- **Example**: `if (ptr == null) { ... }`

#### `byte`
An alias for `char`, used to signify that the data is treated as raw bytes rather than characters.
- **Example**: `byte *buffer = aalloc(1024);`
