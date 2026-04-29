#pragma once

#include "array_list.h"

#include "macros.h"

typedef void *(*iterator_transformer)(void *);

typedef uint8_t (*iterator_filter_predicate)(void *);

enum _iterator_predicate_order {
  map_first,
  filter_first,
  none,
};

struct iterator {
  struct array_list *list;
  byte *ptr;

  byte *ptr2;

  iterator_transformer transformer;
  iterator_filter_predicate filter;
  enum _iterator_predicate_order order;
};

// Example usage:
//  for (struct iterator *it = iterator_begin(al2);
//       iterator_element(it) != iterator_end(it); iterator_increment(it)) {
//    printf("v: %d\n", iterator_element_foo(it)->num);
//  }

struct iterator *iterator_begin(struct array_list *list);

// Returns the address post last element like c++.
void *iterator_end(struct iterator *it);

// The current element stored in it.
byte *iterator_element(struct iterator *it);

void iterator_increment(struct iterator *it);

// TODO: cbegin, cend, rbegin, rend;

struct iterator *iterator_map(struct iterator *it,
                              const iterator_transformer transformer);
struct iterator *iterator_filter(struct iterator *it,
                                 const iterator_filter_predicate transformer);
void *iterator_next(struct iterator *it);
