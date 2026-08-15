// vi: set ts=2 sw=2 expandtab:
#include "arena.h"
#include <stdlib.h>
#include <string.h>

#ifndef ARENA_SIZ
#define ARENA_SIZ 1024
#endif

#include "macros.h"

struct memory_arena {
  byte *chunk;

  byte *ptr;

  size_t size;

  struct memory_arena *next_arena;
};

struct memory_arena arena = {
    .chunk = null,
    .ptr = null,
    .size = 0,
    .next_arena = null,
};

struct memory_arena *current = &arena;

void arena_free() {
  struct memory_arena *curr = arena.next_arena;
  while (null != curr) {
    free(curr->chunk);
    struct memory_arena *to_free = curr;
    curr = curr->next_arena;
    free(to_free);
  }
  free(arena.chunk);
}

void *arena_alloc(size_t size) {
  /// TODO: check on memory alignment
  // size = (size + 7) & ~7;
  //  if head chunk is empty
  if (null == arena.chunk) {
    // sizeof(char) == 1 byte most of the time;
    size_t required_size = size < ARENA_SIZ ? ARENA_SIZ : size * 2;
    arena.chunk = malloc(required_size);
    if (arena.chunk == null) {
      fprintf(stderr, "Failed to initialize Arena\n");
      return null;
    }
    arena.size = required_size;
    arena.ptr = arena.chunk;
    atexit(arena_free);
  }
  if (current->chunk + current->size - current->ptr < size) {
    struct memory_arena *new_arena =
        cast(struct memory_arena *, malloc(sizeof(struct memory_arena)));
    if (null == new_arena) {
      perror("new_arena_allocation");
      return null;
    }
    current->next_arena = new_arena;
    current = new_arena;
    size_t new_size = size > ARENA_SIZ ? size * 2 : ARENA_SIZ;
    current->chunk = malloc(new_size);
    if (current->chunk == null) {
      perror("new_arena_chunk_allocation");
      return null;
    }
    current->ptr = current->chunk;
    current->size = new_size;
    current->next_arena = null;
  }
  if (null == current->chunk || null == current->ptr) {
    perror("no_arena_region");
    return null;
  }
  void *allocated = current->ptr;
  current->ptr += size;
  return allocated;
}

char *string_create_empty(size_t characters) {
  int length = characters + 1;
  char *str = arena_alloc(length * sizeof(char));
  memset(str, 0, length);
  return str;
}
