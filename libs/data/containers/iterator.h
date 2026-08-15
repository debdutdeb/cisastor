#pragma once

#include "arena.h"
#include "macros.h"
#include <stdint.h>

struct iterator;
struct array_list;

typedef void (*iterator_increment_t)(struct iterator *it);
typedef void *(*iterator_element_t)(struct iterator *it);
typedef void *(*iterator_end_t)(struct iterator *it);

struct iterator {
  void *data;
  byte *ptr;

  iterator_increment_t increment;
  iterator_element_t element;
  iterator_end_t end;
};

/*
 * It will be the transformer's responsibility to fill result with the return
 * mutation.
 * Since a mutating iterator does not cross scopes, this result buffer can be
 * stored in stack, if the data is not big. Or reuse the same buffer across each
 * mutation.
 * Map can not modify source data, see const qualifiers.
 * To easily process an element, use `map_copy` and `send_to` to copy current
 * element and send transformed data to result buffer. Follow the pattern below:
 * ```c
 * struct foo* element = map_copy(*current_element);
 * // handle element now
 * send_to(*data, result);
 * ```
 */
typedef void (*iterator_map_predicate)(const void *const current_element,
                                       const void *result);
typedef uint8_t (*iterator_filter_predicate)(const void *const current_element);

struct iterator *iterator_for(void *data, iterator_increment_t increment,
                              iterator_element_t element, iterator_end_t end);

/*
 * Mutating iterators;
 * Mutating iterators applies mutations over an iterator.
 * Two kinds of mutations can be applied -
 * 1. data transformations through map
 * 2. Skipping elements with filter, iterator level mutation
 * Mutations DO NOT MODIFY SOURCE DATA. Keeping original data clean is the most
 * critical part. Use iterator_mutating sparingly. May involve a lot of
 * allocations that are unnecessary. A plain iterator with simple loop may be a
 * better option for most cases.
 */
typedef struct iterator_mutating iterator_mutating;

#define map_copy(val)                                                          \
  __extension__({                                                              \
    __typeof__(val) _temp = (val);                                             \
    __typeof__(val) *_ptr = aalloc(sizeof(__typeof__(val)));                   \
    if (_ptr) {                                                                \
      memcpy(_ptr, &_temp, sizeof(__typeof__(val)));                           \
    }                                                                          \
    _ptr;                                                                      \
  })

#define send_to(val, dst)                                                      \
  __extension__({                                                              \
    __typeof__(val) _temp = (val);                                             \
    memcpy(dst, &_temp, sizeof(_temp));                                        \
  })

// Example usage:
//  for (struct iterator *it = iterator_begin(al2);
//       iterator_element(it) != iterator_end(it); iterator_increment(it)) {
//    printf("v: %d\n", iterator_element_foo(it)->num);
//  }
// This is a helper iterator creator for `array_list`s.
// For any arbitrary data, use `iterator_for` with accessor functions to
// finalize the generic iterator.
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
// Because multiple mutations may be chained, for iterator_mutating to cache an
// element, we'd have to clear cache for all nodes in the chain when
// iterator_mutating is incremented. If any of the middle-iterator is walked,
// this can result in a
void *iterator_mutating_element(struct iterator_mutating *itm);
void iterator_mutating_increment(struct iterator_mutating *itm);
void *iterator_mutating_end(struct iterator_mutating *itm);

#define for_each(it, list)                                                     \
  for (struct iterator *it = iterator_begin(list);                             \
       iterator_end(it) != iterator_element(it); iterator_increment(it))

#define for_each_it(it)                                                        \
  for (; iterator_end(it) != iterator_element(it); iterator_increment(it))
