#include "testing.h"

#include "array_list.h"
#include "iterator.h"
#include <string.h>

IT(should_iterate_over_array_list) {
  struct array_list_int *al = array_list_create_int();
  for (int i = 0; i < 5; i++) {
    array_list_append_int(al, i);
  }

  struct iterator *it = iterator_begin((struct array_list *)al);
  int count = 0;
  while (iterator_element(it) != iterator_end(it)) {
    int *val = iterator_element_int(it);
    cassertf(*val == count, "element mismatch at %d", count);
    iterator_increment(it);
    count++;
  }
  cassertf(count == 5, "expected 5 elements, got %d", count);
}

void int_double_map(const void *const current_element, const void *result) {
  const int *elem = current_element;
  int doubled = (*elem) * 2;
  send_to(doubled, (void *)result);
}

IT(should_map_elements) {
  struct array_list_int *al = array_list_create_int();
  for (int i = 0; i < 5; i++) {
    array_list_append_int(al, i);
  }

  int result_buffer;
  struct iterator_mutating *itm = iterator_map(
      iterator_begin((struct array_list *)al), int_double_map, &result_buffer);

  int count = 0;
  while (iterator_mutating_element(itm) != iterator_mutating_end(itm)) {
    int *val = iterator_mutating_element(itm);
    cassertf(*val == count * 2,
             "mapped value mismatch at %d, expected %d got %d", count, count * 2,
             *val);
    iterator_mutating_increment(itm);
    count++;
  }
  cassertf(count == 5, "expected 5 elements, got %d", count);
}

uint8_t int_is_even_filter(const void *const current_element) {
  const int *elem = current_element;
  return (*elem % 2 == 0);
}

IT(should_filter_elements) {
  struct array_list_int *al = array_list_create_int();
  for (int i = 0; i < 5; i++) {
    array_list_append_int(al, i);
  }

  struct iterator_mutating *itm = iterator_filter(
      iterator_begin((struct array_list *)al), int_is_even_filter);

  int expected[] = {0, 2, 4};
  int count = 0;
  while (iterator_mutating_element(itm) != iterator_mutating_end(itm)) {
    int *val = iterator_mutating_element(itm);
    cassertf(*val == expected[count],
             "filtered value mismatch at %d, expected %d got %d", count,
             expected[count], *val);
    iterator_mutating_increment(itm);
    count++;
  }
  cassertf(count == 3, "expected 3 elements, got %d", count);
}

IT(should_chain_map_and_filter) {
  struct array_list_int *al = array_list_create_int();
  for (int i = 0; i < 10; i++) {
    array_list_append_int(al, i);
  }

  // Chain: filter(even) -> map(double)
  // Evens: 0, 2, 4, 6, 8
  // Doubled: 0, 4, 8, 12, 16

  int result_buffer;
  struct iterator_mutating *itm_filter = iterator_filter(
      iterator_begin((struct array_list *)al), int_is_even_filter);
  struct iterator_mutating *itm_map =
      iterator_mutating_map(itm_filter, int_double_map, &result_buffer);

  int expected[] = {0, 4, 8, 12, 16};
  int count = 0;
  while (iterator_mutating_element(itm_map) != iterator_mutating_end(itm_map)) {
    int *val = iterator_mutating_element(itm_map);
    cassertf(*val == expected[count],
             "chained value mismatch at %d, expected %d got %d", count,
             expected[count], *val);
    iterator_mutating_increment(itm_map);
    count++;
  }
  cassertf(count == 5, "expected 5 elements, got %d", count);
}

IT(should_handle_empty_list) {
  struct array_list_int *al = array_list_create_int();
  struct iterator *it = iterator_begin((struct array_list *)al);
  cassert(iterator_element(it) == iterator_end(it),
          "empty list should have begin == end");
}
