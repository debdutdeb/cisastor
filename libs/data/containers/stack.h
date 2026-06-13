#pragma once

#include "array_list.h"
#include "macros.h"

typedef struct array_list stack;

stack *stack_create(size_t size);

byte *stack_pop(stack *q);
void stack_push(stack *q, byte *data);

size_t stack_size(stack *q);

#define stack_init(type)                                                       \
  typedef stack stack_##type;                                                  \
  static inline stack_##type *stack_create_##type() {                          \
    return stack_create(sizeof(type));                                         \
  }                                                                            \
  static inline void stack_push_##type(stack_##type *q, type data) {            \
    stack_push(q, cast(byte *, &data));                                         \
  }                                                                            \
  static inline void stack_push_##type##_ptr(stack_##type *q, type *data) {     \
    stack_push(q, cast(byte *, data));                                          \
  }                                                                            \
  static inline type *stack_pop_##type(stack_##type *q) {                      \
    return cast(type *, stack_pop(q));                                         \
  }

stack_init(int);
stack_init(char);
