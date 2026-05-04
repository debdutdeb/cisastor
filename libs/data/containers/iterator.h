#pragma once

#include "array_list.h"

#include "macros.h"

struct iterator {
  struct array_list *list;
  byte *ptr;
};

// It will be the transformer's responsibility to fill result with the return
// mutation.
// Since a mutating iterator does not cross scopes, this result buffer can be
// stored in stack, if the data is not big. Or reuse the same buffer across each
// mutation.
typedef void (*iterator_map_predicate)(void *current_element, void *result);
typedef uint8_t (*iterator_filter_predicate)(void *current_element);

typedef struct iterator_mutating iterator_mutating;

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

struct iterator_mutating *iterator_map(struct iterator *it,
                                       iterator_map_predicate predicate,
                                       void *result_buffer);
struct iterator_mutating *iterator_filter(struct iterator *it,
                                          iterator_filter_predicate predicate);

struct iterator_mutating *iterator_mutating_from(struct iterator *it);

struct iterator_mutating *
iterator_mutating_map(struct iterator_mutating *itm,
                      iterator_map_predicate predicate, void *result_buffer);

struct iterator_mutating *
iterator_mutating_filter(struct iterator_mutating *itm,
                         iterator_filter_predicate predicate);

// iterator_mutating_element here differs in behavior from iterator_element.
// Multiple calls to iterator_mutating_element may progress the iterator without
// having to call iterator_mutating_increment. This is because of filter
// mutation, which itself can progress the underlying iterator when a mutation
// is requested from iterator_mutating_element.
// Because multiple mutations may be chained, for iterator_mutating to cache an element, we'd have to clear cache for all nodes in the chain when iterator_mutating is incremented.
// If any of the middle-iterator is walked, this can result in a 
void *iterator_mutating_element(struct iterator_mutating *itm);
void iterator_mutating_increment(struct iterator_mutating *itm);
void *iterator_mutating_end(struct iterator_mutating *itm);
