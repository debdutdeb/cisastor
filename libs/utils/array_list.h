#pragma once

#include <stddef.h>
#include <stdint.h>

// array_list is a dynamic list like slices are in golang
// accessing elements should be o(1)
// stores untyped data, use `cast` to build respective macros.
// e.g.
// #define array_list_get_int_at(a, b) *cast(int, array_list_get_element_at(a,
// b))
struct array_list {
  void *region;

  uint32_t capacity;
  uint32_t length;

  size_t element_size;
};

struct array_list *array_list_create(size_t size);
struct array_list *array_list_create_with_capacity(size_t size,
                                                   uint32_t capacity);
struct array_list *array_list_destroy(struct array_list *al);
void *array_list_get_element_at(struct array_list *al, int index);
// it is highly adviced to use a wrapper for this to enforce the type
// returns a new address for the element inside the array_list
// null if failed;
// e.g.
// int *array_list_append_int(struct array_list *al, int num) {
//   return cast(int *, array_list_append(al, &num));
// }
void* array_list_append(struct array_list *al, const void *data);
