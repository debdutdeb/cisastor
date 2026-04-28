#pragma once

#include "array_list.h"

struct iterator {
  struct array_list *list;
  void *ptr;
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
void *iterator_element(struct iterator *it);

void iterator_increment(struct iterator *it);

// TODO: cbegin, cend, rbegin, rend;
