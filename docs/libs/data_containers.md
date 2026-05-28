# Data Containers

A collection of high-level data structures for C, including dynamic arrays, generic iterators, and string views.

## Array List

`array_list` is a dynamic array that grows automatically. It provides type-safe wrappers for ease of use.

### Usage Example
First, initialize the list for your type, then use the generated functions.

```c
#include "containers/array_list.h"

// Define your type and initialize the array list for it
typedef struct user {
    int id;
} user;
array_list_init(user);

void example() {
    // Create a list
    struct array_list_user *list = array_list_create_user();

    // Append items
    user u1 = { .id = 101 };
    array_list_append_user(list, u1);

    // Access items
    user *retrieved = array_list_get_user_at(list, 0);
    if (retrieved) {
        printf("User ID: %d\n", retrieved->id);
    }
    
    // Check size
    size_t len = array_list_get_length((struct array_list *)list);
}
```

### API Reference (Common)
Note: `type` refers to the type used in `array_list_init(type)`.

| Function | Description |
| --- | --- |
| `array_list_create_type()` | Creates a new list with default capacity. |
| `array_list_create_type_with_capacity(uint32_t cap)` | Creates a list with specified initial capacity. |
| `array_list_append_type(list, val)` | Appends a value (copied by value). Returns pointer to new element. |
| `array_list_get_type_at(list, index)` | Returns a pointer to the element at `index`, or `NULL` if OOB. |
| `array_list_get_length(list)` | Returns current number of elements. (Requires cast to `struct array_list *`) |

---

## Iterator

Iterators provide a uniform way to traverse containers and apply transformations like `map` and `filter`.

### Basic Iteration
```c
#include "containers/iterator.h"

void print_all(struct array_list_int *list) {
    struct iterator *it = iterator_begin((struct array_list *)list);
    
    while (iterator_element(it) != iterator_end(it)) {
        int *val = iterator_element_int(it);
        printf("%d\n", *val);
        iterator_increment(it);
    }
}
```

### Functional Transformations (Map & Filter)
You can chain operations without modifying the original data.

```c
void double_val(const void *src, const void *dst) {
    int val = *(int*)src;
    int result = val * 2;
    send_to(result, (void*)dst);
}

void example_map(struct array_list_int *list) {
    int result_buf;
    struct iterator_mutating *itm = iterator_map(
        iterator_begin((struct array_list *)list), 
        double_val, 
        &result_buf
    );

    // Iterating 'itm' returns the doubled values
}
```

---

## String View

A `string_view` is an efficient, non-owning way to read and parse strings.

### Usage Example
```c
#include "containers/string_view.h"

void parse_config(char *input) {
    struct string_view *sv = string_view_create(input);

    string_view_consume_whitespace(sv);
    if (string_view_compare_word(sv, "key") == 0) {
        // 'key' matched, pointer moved past it
        string_view_consume_chars(sv, " ="); // consume optional spaces and equals
        const char *val = string_view_consume_word(sv);
        printf("Value: %s\n", val);
    }
}
```

### Parsing API Reference

| Function | Description |
| --- | --- |
| `string_view_peek_char(sv)` | Returns current char without moving. |
| `string_view_next_char(sv)` | Returns current char and moves forward. |
| `string_view_compare_word(sv, word)` | Returns 0 and advances if `word` matches next chars. |
| `string_view_consume_whitespace(sv)` | Skips all spaces and tabs. |
| `string_view_consume_word(sv)` | Consumes and returns the next alphanumeric word. |
| `string_consume_till_end_of_line(sv)` | Advances to the next newline. |
