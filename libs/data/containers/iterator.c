#include "iterator.h"
#include "arena.h"
#include "array_list.h"
#include "macros.h"

struct iterator_mutating_map {
  iterator_map_predicate transform;
  char *buffer;
};
struct iterator_mutating_filter {
  iterator_filter_predicate filter;
};

// One or the other, no need for pointers here, defeats the purpose of a union
// then;
union iterator_mutating_mutator {
  struct iterator_mutating_map map;
  struct iterator_mutating_filter filter;
};

enum iterator_mutating_kind {
  map,
  filter,
};

struct iterator_mutating {
  enum iterator_mutating_kind kind;
  union iterator_mutating_mutator *mutator;
  struct iterator_mutating *parent;
  struct iterator *it;
  // temporary buffer to store *_element
  void *buffer;
};

struct iterator *iterator_begin(struct array_list *list) {
  struct iterator *it = aalloc(sizeof(struct iterator));
  if (null == it) {
    return null;
  }
  it->list = list;
  it->ptr = array_list_get_element_at(list, 0);
  return it;
}

void *iterator_end(struct iterator *it) {
  return &it->list->region[it->list->length * it->list->element_size];
}

byte *iterator_element(struct iterator *it) { return it->ptr; }

void iterator_increment(struct iterator *it) {
  it->ptr += it->list->element_size;
}

struct iterator_mutating *iterator_mutating_from(struct iterator *it) {
  struct iterator_mutating *itm = aalloc(sizeof(struct iterator_mutating));
  if (null == itm) {
    return null;
  }
  itm->mutator = aalloc((sizeof(union iterator_mutating_mutator)));
  if (null == itm->mutator) {
    fprintf(stderr, "mutator_allocation_failed");
    return null;
  }
  itm->it = it;
  itm->buffer = null;
  itm->parent = null;
  return itm;
}

struct iterator_mutating *
iterator_mutating_map(struct iterator_mutating *itm,
                      iterator_map_predicate predicate, void *result_buffer) {
  if (null == result_buffer) {
    fprintf(
        stderr,
        "iterator_mutating_map: unallocated result buffer passed, addr: %p\n",
        result_buffer);
    return null;
  }
  struct iterator_mutating *itm2 = iterator_mutating_from(itm->it);
  if (null == itm2) {
    return null;
  }
  itm2->parent = itm;
  // inherit the iterator
  itm2->it = itm->it;
  itm2->mutator->map.transform = predicate;
  itm2->mutator->map.buffer = result_buffer;
  itm2->kind = map;
  return itm2;
}

struct iterator_mutating *
iterator_mutating_filter(struct iterator_mutating *itm,
                         iterator_filter_predicate predicate) {
  struct iterator_mutating *itm2 = iterator_mutating_from(itm->it);
  if (null == itm2) {
    return null;
  }
  itm2->parent = itm;
  itm2->it = itm->it;
  itm2->mutator->filter.filter = predicate;
  itm2->kind = filter;
  return itm2;
}

void *iterator_mutating_end(struct iterator_mutating *itm) {
  return iterator_end(itm->it);
}

uint8_t iterator_ended(struct iterator *it) {
  return cast(byte *, iterator_end(it)) <= cast(byte *, it->ptr);
}

void *iterator_mutating_element(struct iterator_mutating *itm) {
  struct iterator *it = itm->it;
  if (iterator_ended(it)) {
    return iterator_end(it);
  }
  if (null != itm->buffer) {
    return itm->buffer;
  }
  switch (itm->kind) {
  case map: {
    void *element = iterator_element(it);
    if (itm->parent) {
      // recursively get the mutated next element.
      element = iterator_mutating_element(itm->parent);
      // chain of mutating iterators share underlying iterator.
      if (iterator_ended(it)) {
        return iterator_end(it);
      }
    }
    // apply current mutator
    itm->mutator->map.transform(element, itm->mutator->map.buffer);
    itm->buffer = itm->mutator->map.buffer;
    return itm->buffer;
  }
  case filter: {
    for (; !iterator_ended(it); iterator_mutating_increment(itm)) {
      void *element = iterator_element(it);
      if (itm->parent) {
        element = iterator_mutating_element(itm->parent);
        if (iterator_ended(it)) {
          return iterator_end(it);
        }
      }
      if (itm->mutator->filter.filter(element)) {
        itm->buffer = element;
        return itm->buffer;
      }
    }
    break;
  }

  default:
    fprintf(stderr, "unknown type of mutating iterator %d\n", itm->kind);
    return null;
  }
  return iterator_end(itm->it);
}

void iterator_mutating_increment(struct iterator_mutating *itm) {
  struct iterator_mutating *curr = itm;
  while (null != curr) {
    curr->buffer = null;
    curr = curr->parent;
  }
  iterator_increment(itm->it);
}

struct iterator_mutating *iterator_map(struct iterator *it,
                                       iterator_map_predicate predicate,
                                       void *result_buffer) {
  if (null == result_buffer) {
    fprintf(stderr,
            "iterator_map: unallocated result buffer passed, addr: %p\n",
            result_buffer);
    return null;
  }
  struct iterator_mutating *itm = iterator_mutating_from(it);
  if (null == itm) {
    return itm;
  }
  itm->mutator->map.transform = predicate;
  itm->mutator->map.buffer = result_buffer;
  return itm;
}
struct iterator_mutating *iterator_filter(struct iterator *it,
                                          iterator_filter_predicate predicate) {
  struct iterator_mutating *itm = iterator_mutating_from(it);
  if (null == itm) {
    return itm;
  }
  itm->mutator->filter.filter = predicate;
  itm->kind = filter;
  return itm;
}
