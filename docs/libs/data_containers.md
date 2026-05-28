# Data Containers

Collection of generic data structures and utilities.

## Array List

A dynamic array implementation similar to Go slices or C++ `std::vector`.

### Type Safe API Generation
Use the `array_list_init(type)` macro to generate type-safe wrapper functions.

```c
typedef struct foo { int id; } foo;
array_list_init(foo);

// Generated functions:
struct array_list_foo *al = array_list_create_foo();
foo f = {1};
array_list_append_foo(al, f);
foo *ptr = array_list_get_foo_at(al, 0);
```

### Core API

#### `size_t array_list_get_length(struct array_list *al)`
Returns the current number of elements.

#### `size_t array_list_get_capacity(struct array_list *al)`
Returns the current allocated capacity.

#### `byte *array_list_get_element_at(struct array_list *al, int index)`
Returns a pointer to the element at the given index, or `NULL` if out of bounds.

#### `byte *array_list_append(struct array_list *al, const void *data)`
Appends data to the list, reallocating if necessary. Returns a pointer to the newly added element.

---

## Iterator

A generic iterator pattern for traversing containers.

### Core API

#### `struct iterator *iterator_begin(struct array_list *list)`
Creates an iterator for an `array_list`.

#### `void *iterator_end(struct iterator *it)`
Returns a sentinel pointer representing the end of the container.

#### `byte *iterator_element(struct iterator *it)`
Returns the current element.

#### `void iterator_increment(struct iterator *it)`
Advances the iterator.

### Mutating Iterators

#### `struct iterator_mutating *iterator_map(struct iterator *it, iterator_map_predicate predicate, void *result_buffer)`
Creates a mapping iterator.

#### `struct iterator_mutating *iterator_filter(struct iterator *it, iterator_filter_predicate predicate)`
Creates a filtering iterator.

---

## String View

A non-owning view into a string, useful for parsing.

### Core API

#### `struct string_view *string_view_create(char *stream)`
Creates a view for the given string.

#### `char string_view_peek_char(struct string_view *sv)`
Returns the current character without advancing the pointer.

#### `char string_view_next_char(struct string_view *sv)`
Returns the current character and advances the pointer.

#### `int string_view_current_pos(struct string_view *sv)`
Returns the index of the last character read (-1 initially).

#### `char string_view_char_at(struct string_view *sv, int pos)`
Returns the character at the absolute position in the original stream.

#### `int string_view_compare_word(struct string_view *sv, const char *const word)`
Checks if the next characters match `word`. Advances pointer on match. Returns 0 on success.

### Consumption API

#### `void string_view_consume_whitespace(struct string_view *sv)`
Skips spaces and tabs.

#### `const char *const string_view_consume_word(struct string_view *sv)`
Consumes and returns an alphanumeric (plus underscore) word.

#### `void string_consume_till_end_of_line(struct string_view *sv)`
Advances the pointer to the next newline or end of string.
