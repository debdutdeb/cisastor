// vi: set ts=2 sw=2 expandtab:
#include "arena.h"

#ifndef ARENA_SIZ
#define ARENA_SIZ 1024
#endif

#include "macros.h"

struct memory_arena {
  char *chunk;

  char *ptr;

  int current_size;
};

struct arena_allocations {
  void *ptr;
  size_t size;
  struct arena_allocations *next;
};

struct arena_allocations allocations = {.ptr = null, .size = 0, .next = null};

struct memory_arena arena = {};

void arena_free() { free(arena.chunk); }

void *arena_alloc(size_t size) {
  if (null == arena.chunk) {
    // sizeof(char) == 1 byte most of the time;
    arena.chunk = malloc(sizeof(char) * ARENA_SIZ);
    if (null == arena.chunk) {
      perror("arena_alloc");
      return null;
    }

    arena.current_size = ARENA_SIZ;
    arena.ptr = arena.chunk;

    atexit(cast(void (*)(void), arena_free));
  }

  void *ptr = arena.ptr;
  void *chunk = arena.chunk;

  void *end = chunk + arena.current_size;

  if (end - ptr < size) {
    size_t allocated_region = ptr - chunk;
    arena.chunk =
        realloc(arena.chunk, arena.current_size + (ARENA_SIZ * sizeof(char)));
    if (null == arena.chunk) {
      perror("arena_realloc");
      return null;
    }

    arena.current_size += ARENA_SIZ;
    end = arena.chunk + arena.current_size;
    arena.ptr = arena.chunk + allocated_region;
  }

  void *allocated = arena.ptr;

  arena.ptr += size;

  return allocated;
}

char *string_create_empty(size_t characters) {
  int length = characters + 1;
  char *str = arena_alloc(length * sizeof(char));
  memset(str, 0, length);
  return str;
}

