#include "iterator.h"
#include "arena.h"
#include "array_list.h"
#include "macros.h"

struct iterator *iterator_begin(struct array_list *list) {
  struct iterator *it = aalloc(sizeof(struct iterator));
  if (null == it) {
    return null;
  }
  it->list = list;
  it->ptr = array_list_get_element_at(list, 0);
  it->transformer = null;
  it->filter = null;
  it->order = none;
  it->ptr2 = null;
  return it;
}

void *iterator_end(struct iterator *it) {
  return &it->list->region[it->list->length * it->list->element_size];
}

byte *iterator_element(struct iterator *it) {
  if (null != it->ptr2) {
    return it->ptr2;
  }
  if (cast(char *, iterator_end(it)) <= it->ptr) {
    return iterator_end(it);
  }
  switch (it->order) {
  case none:
    return it->ptr;
  case map_first: {
    while (it->ptr != iterator_end(it)) {
      void *result = it->transformer(it->ptr);
      if (null != it->filter) {
        if (it->filter(result)) {
          it->ptr2 = result;
          return result;
        }
        iterator_increment(it);
        continue;
      }
      it->ptr2 = result;
      return result;
    }
    break;
  }
  case filter_first: {
    while (iterator_end(it) != it->ptr) {
      if (!it->filter(it->ptr)) {
        iterator_increment(it);
        continue;
      }
      if (null != it->transformer) {
        it->ptr2 = it->transformer(it->ptr);
        return it->ptr2;
      }
      it->ptr2 = it->ptr;
      return it->ptr;
    }
    break;
  }
  }
  return iterator_end(it);
}

void iterator_increment(struct iterator *it) {
  it->ptr2 = null;
  it->ptr += it->list->element_size;
}

struct iterator *iterator_map(struct iterator *it,
                              const iterator_transformer transformer) {
  it->transformer = transformer;
  if (it->order == none) {
    it->order = map_first;
  }
  return it;
}

struct iterator *iterator_filter(struct iterator *it,
                                 const iterator_filter_predicate filter) {
  it->filter = filter;
  if (it->order == none) {
    it->order = filter_first;
  }
  return it;
}
