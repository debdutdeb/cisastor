#pragma once

#include <stddef.h>
#include <stdint.h>

#include "iterator.h"
#include "macros.h"

// array_list is a dynamic list like slices are in golang
// accessing elements should be o(1)
// stores untyped data.
struct array_list {
  byte *region;

  uint32_t capacity;
  uint32_t length;

  size_t element_size;
};

// array_list_init is to provide type safe functions derived for array_list
// operations. E.g. array_list_init(int); // will provide access to
// array_list_create_int, array_list_get_int_at, etc so no need to cast manually
// and pass sizeofs everywhere; For structs, use typedef like struct foo {
//   int num;
// };
// typedef struct foo foo;
// array_list_init(foo);
//  // now use the expanded functions as so -
// struct array_list *al2 = array_list_create_foo();
// assert(null != al2);
// struct foo f = {.num = 1};
// array_list_append_foo(al2, f);
// f.num = 2;
// array_list_append_foo(al2, f);
// printf("at 1 %d\n", array_list_get_foo_at(al2, 1).num);
// Read examples/array_list.c for more.
#define array_list_init(type)                                                  \
  static inline struct array_list *array_list_create_##type() {                \
    return array_list_create(sizeof(type));                                    \
  }                                                                            \
                                                                               \
  static inline struct array_list *array_list_create_##type##_with_capacity(   \
      uint32_t capacity) {                                                     \
    return array_list_create_with_capacity(sizeof(type), capacity);            \
  }                                                                            \
                                                                               \
  static inline type *array_list_get_##type##_at(struct array_list *al,        \
                                                 int index) {                  \
    return cast(type *, array_list_get_element_at(al, index));                 \
  }                                                                            \
                                                                               \
  static inline type *array_list_append_##type(struct array_list *al,          \
                                               const type data) {              \
    return cast(type *, array_list_append(al, cast(const char *, &data)));     \
  }                                                                            \
                                                                               \
  static inline type *iterator_element_##type(struct iterator *it) {           \
    return cast(type *, iterator_element(it));                                 \
  }

struct array_list *array_list_create(size_t size);
struct array_list *array_list_create_with_capacity(size_t size,
                                                   uint32_t capacity);
struct array_list *array_list_destroy(struct array_list *al);
byte *array_list_get_element_at(struct array_list *al, int index);
byte *array_list_append(struct array_list *al, const byte *data);
