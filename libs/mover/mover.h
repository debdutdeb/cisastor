#pragma once

#include <string.h>
#include <stdint.h>
#include <stddef.h>

#include "macros.h"

typedef struct mover {
  byte *data;
  size_t size;
} mover_t;

mover_t *mover_claim(byte *data, size_t size);
mover_t *mover_move(mover_t *mover);
byte *mover_borrow(mover_t *mover);

#define __mover_init(type)                                                     \
  typedef mover_t type##_mover_t;                                              \
  static inline type##_mover_t *mover_claim_##type(type *data) {               \
    return mover_claim((byte *)data, sizeof(type));                            \
  }                                                                            \
  static inline type##_mover_t *mover_move_##type(type##_mover_t *mover) {     \
    return mover_move(mover);                                                  \
  }                                                                            \
  static inline type mover_borrow_##type(type##_mover_t *mover) {              \
    byte *b = mover_borrow(mover);                                             \
    if (b == null) {                                                           \
      type _zero;                                                              \
      memset(&_zero, 0, sizeof(type));                                         \
      return _zero;                                                            \
    }                                                                          \
    return *(type *)b;                                                         \
  }

__mover_init(int);
__mover_init(char);
__mover_init(float);
__mover_init(double);
__mover_init(long);
typedef long long llong;
__mover_init(llong);
__mover_init(uint8_t);
__mover_init(uint16_t);
__mover_init(uint32_t);
__mover_init(uint64_t);
__mover_init(int8_t);
__mover_init(int16_t);
__mover_init(int32_t);
__mover_init(int64_t);
__mover_init(size_t);

// For pointers
typedef void *void_ptr_t;
__mover_init(void_ptr_t);
typedef int *int_ptr_t;
__mover_init(int_ptr_t);
typedef char *char_ptr_t;
__mover_init(char_ptr_t);
typedef float *float_ptr_t;
__mover_init(float_ptr_t);
typedef double *double_ptr_t;
__mover_init(double_ptr_t);

// Nested mover support
__mover_init(mover_t);
