# Iterator

The Iterator library provides a uniform interface for traversing containers (like `array_list`) and applying functional transformations. It is heavily inspired by patterns found in Go, C++, and functional programming languages.

## Why Iterators?

Directly looping over container internals (like `list->region`) makes code rigid and dependent on the container's implementation. Iterators solve this by:
- **Abstraction**: You can write code that works on any data source that implements the iterator interface.
- **Functional Composition**: You can chain `map` and `filter` operations to process data in a pipeline without modifying the original container.
- **Safety**: Iterators track their own state, reducing the risk of off-by-one errors in manual loops.

## Basic Traversal

Every iterator provides a current `element`, an `increment` function, and an `end` sentinel.

```c
#include "containers/iterator.h"

struct iterator *it = iterator_begin(my_list);
while (iterator_element(it) != iterator_end(it)) {
    // Process element
    void *elem = iterator_element(it);
    
    // Move to next
    iterator_increment(it);
}
```

## Mutating Iterators (Map & Filter)

"Mutating" iterators apply transformations to the data as it is being walked. **Crucially, they do not modify the source data.** Instead, they provide a "view" of the transformed data.

### The Result Buffer
Because C does not have a garbage collector, `map` operations require a **Result Buffer**. 
- When the iterator's `element` is requested, the transformation function is called.
- The result of this transformation is written into a buffer provided by the caller.
- This buffer can be a local variable on the stack or a shared buffer reused for every element to save allocations.

### Helper Macros

#### `map_copy(val)`
Used inside a `map` predicate to create a temporary, arena-allocated copy of the current element. This is useful if you need to work with the value safely before sending it to the result.

#### `send_to(val, dst)`
Used to write the final transformed value into the result buffer.
- **Example**: `send_to(doubled_value, result);`

### Chaining Example
You can chain multiple mutations. For example, filtering even numbers and then doubling them:

```c
// 1. Filter predicate
uint8_t is_even(const void *elem) {
    return (*(int*)elem % 2 == 0);
}

// 2. Map predicate
void double_val(const void *src, const void *dst) {
    int result = (*(int*)src) * 2;
    send_to(result, (void*)dst);
}

void pipeline(struct array_list_int *list) {
    int buffer; // Shared result buffer
    
    // Create filter
    struct iterator_mutating *filtered = iterator_filter(iterator_begin(list), is_even);
    
    // Chain map onto filter
    struct iterator_mutating *mapped = iterator_mutating_map(filtered, double_val, &buffer);

    // Iterating 'mapped' will now yield: 0, 4, 8, 12... (doubled evens)
}
```

## API Reference

| Function / Macro | Description |
| --- | --- |
| `iterator_begin(list)` | Starts iteration over an `array_list`. |
| `iterator_element(it)` | Returns a pointer to the current element. |
| `iterator_increment(it)` | Advances the iterator. |
| `iterator_map(it, pred, buf)` | Creates a mapping iterator from a base iterator. |
| `iterator_filter(it, pred)` | Creates a filtering iterator from a base iterator. |
| `iterator_mutating_map(...)` | Chains a map onto an existing mutating iterator. |
| `iterator_mutating_filter(...)` | Chains a filter onto an existing mutating iterator. |
| `send_to(val, dst)` | Macro to copy `val` into the destination buffer `dst`. |
