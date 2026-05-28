# String View

A `string_view` is a stateful, non-owning "window" into a character stream. It is designed for high-performance parsing and string manipulation.

## Why String View?

Traditional C string functions often require modifying the original string (like `strtok`) or frequent allocations and copies (like `strncpy`). `string_view` solves this by:
- **Efficiency**: It never copies the underlying characters. It only moves a pointer.
- **Stateful Parsing**: It maintains a "current position", making it ideal for writing lexers and parsers where you need to "consume" tokens sequentially.
- **Non-Destructive**: You can parse a read-only string buffer without changing it.

## Usage

### Creation and Peeking
```c
#include "containers/string_view.h"

char *input = "hello:world";
struct string_view *sv = string_view_create(input);

// Peek doesn't move the pointer
if (string_view_peek_char(sv) == 'h') {
    // Next char moves the pointer
    char c = string_view_next_char(sv); // c = 'h'
}
```

### Consumption Patterns
The "Consume" API is the core of the library. It allows you to skip or extract parts of the string based on patterns.

```c
void parse_example(char *input) {
    struct string_view *sv = string_view_create(input);

    // Skip spaces and tabs
    string_view_consume_whitespace(sv);

    // Extract a word (alphanumeric + '_')
    // This DOES allocate a new string in the Arena for the result
    const char *key = string_view_consume_word(sv);

    // Check for a specific delimiter and skip it
    if (string_view_peek_char(sv) == ':') {
        string_view_next_char(sv);
    }
}
```

## API Reference

### Core Navigation
| Function | Description |
| --- | --- |
| `string_view_peek_char(sv)` | Look at the current character without advancing. |
| `string_view_next_char(sv)` | Return the current character and move to the next. |
| `string_view_current_pos(sv)` | Return the 0-based index of the *last read* character. |
| `string_view_char_at(sv, pos)` | Look at any character in the original stream by absolute index. |

### Comparison and Consumption
| Function | Description |
| --- | --- |
| `string_view_compare_word(sv, word)` | Checks if `word` matches current position. If yes, advances past it and returns 0. |
| `string_view_consume_whitespace(sv)` | Advances past any spaces or tabs. |
| `string_view_consume_word(sv)` | Extracts an alphanumeric word into a new arena-managed string. |
| `string_consume_till_end_of_line(sv)` | Advances until a newline or null terminator is reached. |
| `string_view_consume_chars(sv, list)` | Advances as long as the current char is in the provided `list`. |
| `string_view_consume_word_until_chars(sv, list)` | Extracts everything until it hits any character in `list`. |
