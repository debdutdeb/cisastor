#pragma once

#include "array_list.h"
#include "macros.h"

typedef struct array_list stack;

stack *stack_create(size_t size);

byte *stack_pop(stack *);
byte *stack_peek(stack *);
void stack_push(stack *, byte *data);

size_t stack_size(stack *);

#define stack_init(type)                                                       \
  typedef stack stack_##type;                                                  \
  static inline stack_##type *stack_create_##type() {                          \
    return stack_create(sizeof(type));                                         \
  }                                                                            \
  static inline void stack_push_##type(stack_##type *s, type data) {           \
    stack_push(s, cast(byte *, &data));                                        \
  }                                                                            \
  static inline void stack_push_##type##_ptr(stack_##type *s, type *data) {    \
    stack_push(s, cast(byte *, data));                                         \
  }                                                                            \
  static inline type *stack_pop_##type(stack_##type *s) {                      \
    return cast(type *, stack_pop(s));                                         \
  }                                                                            \
  static inline type *stack_peek_##type(stack_##type *s) {                     \
    return cast(type *, stack_peek(s));                                        \
  }

stack_init(int);
stack_init(char);
