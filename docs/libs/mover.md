# Mover

The `mover` library provides explicit ownership tracking and move semantics for C. Inspired by Rust's memory model, it helps developers manage object lifetimes and ownership transitions through runtime enforcement.

## Overview

In standard C, memory ownership is often implicit and documented only in comments. `mover` makes ownership explicit by wrapping data in a container that can be "claimed" or "moved." When ownership is transferred, the original container is invalidated, preventing accidental reuse.

Typical use cases include:
* Tracking the lifecycle of complex resources.
* Safely passing ownership across thread boundaries or complex function call chains.
* Implementing resource-safe data structures.

## Why?

Manual memory management is prone to errors such as use-after-free, double-free, and memory leaks. `mover` addresses these by:
1. **Explicit Transitions**: Using `mover_move` clearly signals a transfer of responsibility.
2. **Runtime Invalidation**: Unlike raw pointers, `mover` physically relocates data and scrubs the old location on every move, making "use-after-move" bugs obvious and immediate.

### Trade-offs
* **Safety vs. Performance**: Every move operation involves a new allocation and a memory copy. This library prioritizes safety and debuggability over absolute performance.
* **Discipline**: While `mover` provides runtime safety, it still requires developer discipline as C cannot enforce these rules at compile time.

## Concepts

### Ownership
Data is owned by exactly one `mover_t` at any time. To transfer ownership, you must call `mover_move`.

### Physical Move and Scrubbing
A "move" in this library is literal. The data is copied to a new memory address, and the old memory address is zeroed out (scrubbed). This ensures that any "borrows" (pointers to the old location) are immediately invalidated.

### Borrowing
A "borrow" is a temporary reference to the data. It is obtained via `mover_borrow`. A borrowed reference is only valid until the next move operation occurs on that mover.

## Quick Start

```c
#include "mover.h"
#include <stdio.h>

int main() {
    int val = 42;
    // Claim ownership of a copy of val
    int_mover_t *m1 = mover_claim_int(&val);

    // Transfer ownership to m2
    int_mover_t *m2 = mover_move_int(m1);

    // m1 is now invalid. Use m2 to access the value.
    printf("Value: %d\n", mover_borrow_int(m2));

    return 0;
}
```

## Common Usage Patterns

### Passing Ownership to a Function
A function that takes a `mover_t *` should immediately move it to signify taking ownership.

```c
void consume_resource(int_mover_t *m) {
    int_mover_t *owned = mover_move_int(m);
    // ... work with owned ...
}
```

### Nested Ownership
Movers can own other movers, allowing for hierarchical resource management.

```c
int_mover_t *inner = mover_claim_int(&x);
mover_t_mover_t *outer = mover_claim_mover_t(inner);

// Moving 'outer' relocates the 'inner' handle itself
mover_t_mover_t *outer_moved = mover_move_mover_t(outer);
```

## Memory Ownership

* **Allocation**: All functions allocate memory using the configured allocator (defaults to `aalloc` from the `arena` library).
* **Copying**: `mover_claim` and `mover_move` always perform a deep copy of the data they manage.
* **Invalidation**: On `mover_move`, the source mover's data pointer is set to `NULL`, and the old memory is zero-filled.
* **Arena Behavior**: Since it uses `arena_alloc`, memory is managed as part of the current arena's lifecycle and is not manually freed.

## API Reference

### mover_claim

Creates a new mover and claims ownership of a copy of the data.

```c
mover_t *mover_claim(byte *data, size_t size);
```

* **Parameters**: `data` pointer to source, `size` in bytes.
* **Return Value**: A new `mover_t` pointer.
* **Ownership**: The returned mover owns the new copy of the data.
* **Failure**: Returns `NULL` on allocation failure.

### mover_move

Transfers ownership of the data to a new mover.

```c
mover_t *mover_move(mover_t *mover);
```

* **Parameters**: The source `mover`.
* **Return Value**: A new `mover_t` pointer owning the relocated data.
* **Ownership**: The original `mover` is invalidated (data set to `NULL`).
* **Failure**: Returns `NULL` if source is `NULL` or already moved.

### mover_borrow

Provides a temporary pointer to the managed data.

```c
byte *mover_borrow(mover_t *mover);
```

* **Parameters**: The `mover` to borrow from.
* **Return Value**: Pointer to the underlying data.
* **Ownership**: Returns a reference only. Does not transfer ownership.
* **Safety**: Returns `NULL` if the mover has been invalidated.

### Type-Safe Wrappers

Generated via `__mover_init(type)`. Provides `mover_claim_##type`, `mover_move_##type`, and `mover_borrow_##type`.

Example: `int_mover_t *mi = mover_claim_int(&x);`

## Common Pitfalls

* **Dangling Borrows**: Storing the result of `mover_borrow` and then calling `mover_move`. The stored pointer will now point to zeroed memory.
* **Double Moves**: Attempting to move a mover that has already been moved. This returns `NULL`.
* **Zero-Value Assumption**: `mover_borrow_type` returns a zero-initialized struct or `0` if the mover is invalid. Do not confuse this with a valid `0` value.

## Performance Characteristics

* **Time Complexity**: Move operations are O(N) where N is the size of the data, due to `memmove`.
* **Allocation**: Each claim and move triggers two allocations (one for the `mover_t` struct, one for the data).
* **Memory Footprint**: Every move doubles the peak memory usage temporarily until the arena is cleared.

## Related Libraries

* **arena**: `mover` relies on `arena_alloc` for all its memory needs.
* **utils**: Uses `byte`, `cast`, and `null` macros.
* **data**: Movers are often used as elements within `array_list` or other containers to track element ownership.
