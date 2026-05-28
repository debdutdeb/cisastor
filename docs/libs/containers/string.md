# String

The `string` type in Cisastor is a dynamic, growable string built on top of the `array_list` container. Unlike standard C strings, it tracks its length explicitly and handles memory management via the Arena.

## Why String?

- **Automatic Memory Management**: Integrated with the `arena_allocator`, so you don't need to manually `free` strings.
- **Dynamic Growth**: Easily append characters without worrying about buffer overflows or manual `realloc`.
- **Safety**: Functions like `string_char_at` and `string_substring` provide controlled access to string content.
- **Explicit Length**: O(1) length retrieval, avoiding the overhead of `strlen`.

## Usage

### Basic Operations
```c
#include "containers/string.h"

void example() {
    // 1. Create a string from a C literal
    string *s1 = string_create("Hello");
    
    // 2. Append characters
    string_append_char(s1, '!');
    
    // 3. Get length
    size_t len = string_length(s1); // 6
    
    // 4. Access characters
    char c = string_char_at(s1, 0); // 'H'
}
```

### Concatenation and Substrings
```c
void transform_example() {
    string *s1 = string_create("Cisastor");
    string *s2 = string_create(" is cool");
    
    // Concatenate (returns a new string)
    string *res = string_concat(s1, s2);
    
    // Extract substring (returns a new string)
    // string_substring(str, start_index, end_index)
    string *sub = string_substring(res, 0, 8); // "Cisastor"
}
```

### Integration with C APIs
If you need to pass a `string` to a standard C function (like `printf`), use `string_to_primitive`.

```c
string *s = string_create("Standard C");
char *cstr = string_to_primitive(s); // Returns an arena-managed, null-terminated char*
printf("Message: %s\n", cstr);
```

## API Reference

| Function / Macro | Description |
| --- | --- |
| `string_create(literal)` | Creates a `string` from a null-terminated C string. |
| `string_length(str)` | Returns the number of characters in the string. |
| `string_char_at(str, idx)` | Returns the character at the specified index. |
| `string_append_char(str, c)` | Appends a character to the string. |
| `string_concat(s1, s2)` | Returns a new string containing `s1` followed by `s2`. |
| `string_substring(s, i1, i2)` | Returns a new string with characters from index `i1` up to (but not including) `i2`. |
| `string_cmp(s1, s2)` | Compares two strings. Returns 0 if equal, -1 if `s1 < s2`, 1 if `s1 > s2`. |
| `string_to_primitive(str)` | Returns a null-terminated `char*` copy of the string in the Arena. |
