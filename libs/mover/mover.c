#include "mover.h"
#include "arena.h"
#include "macros.h"
#include <string.h>

// allow external allocator
#ifndef CISASTOR_USE_ALLOCATOR
#define alloc aalloc
#else
#define alloc CISASTOR_USE_ALLOCATOR
#endif

mover_t *mover_claim(byte *data, size_t size) {
  mover_t *mover = alloc(sizeof(mover_t));
  if (mover == null) {
    return null;
  }
  byte *mover_data = alloc(size);
  if (mover_data == null) {
    return null;
  }
  memmove(mover_data, data, size);
  mover->data = mover_data;
  mover->size = size;
  return mover;
}

mover_t *mover_move(mover_t *mover) {
  if (mover == null || mover->data == null) {
    return null;
  }
  
  mover_t *mover_moved = alloc(sizeof(mover_t));
  if (mover_moved == null) {
    return null;
  }

  byte *new_data = alloc(mover->size);
  if (new_data == null) {
    return null;
  }

  // Physical move of data to a new location
  memmove(new_data, mover->data, mover->size);
  
  mover_moved->data = new_data;
  mover_moved->size = mover->size;

  // Invalidate original buffer to catch dangling borrows
  memset(mover->data, 0, mover->size);
  mover->data = null;
  mover->size = 0;

  return mover_moved;
}

byte *mover_borrow(mover_t *mover) {
  if (mover == null) {
    return null;
  }
  return mover->data;
}
