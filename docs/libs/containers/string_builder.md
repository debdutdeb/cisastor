# String Builder

`string_builder` is a utility for efficiently concatenating multiple strings together. It is built on top of `array_list` to dynamically collect string pointers, preventing the O(N^2) reallocation overhead associated with repeatedly appending parts.

## Why String Builder?

When you construct a string piece by piece (e.g., in a loop), using `strcat` or `string_concat` repeatedly results in poor performance because the intermediate strings keep getting copied. `string_builder` solves this by:
- **Deferred Allocation**: It collects string parts and delays calculating the exact required capacity until `string_builder_build` is invoked, preventing repeated memory reallocations.
- **Single Pass Construction**: Allocates the exact needed memory once and copies all parts linearly.
- **Dynamic Collection**: Under the hood, it uses an `array_list` of `const char *` to securely and dynamically manage an arbitrary number of references.

## Usage

```c
#include "containers/string_builder.h"
#include "containers/string.h"

void example() {
    // 1. Create the builder (Note: string_builder is a typedef)
    string_builder *sb = string_builder_create();
    
    // 2. Join string literals or C-strings
    string_builder_join(sb, "Hello, ");
    string_builder_join(sb, "world");
    string_builder_join(sb, "!");
    
    // 3. Build the final dynamic `string`
    string *final_str = string_builder_build(sb);
    
    // Output: "Hello, world!"
}
```

## API Reference

| Function | Description |
| --- | --- |
| `string_builder_create()` | Creates a new `string_builder` instance. |
| `string_builder_join(sb, cstr)` | Adds a null-terminated C-string to the builder. Does not copy the string content, only stores the pointer. |
| `string_builder_build(sb)` | Calculates the final required length, allocates the memory, and constructs the concatenated `string` from all joined parts. |
