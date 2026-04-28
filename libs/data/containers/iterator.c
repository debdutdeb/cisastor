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
  return it;
}

void *iterator_end(struct iterator *it) {
  return &it->list->region[it->list->length * it->list->element_size];
}

void *iterator_element(struct iterator *it) { return it->ptr; }

void iterator_increment(struct iterator *it) {
  it->ptr += it->list->element_size;
}
