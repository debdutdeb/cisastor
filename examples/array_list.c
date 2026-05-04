#include "containers/array_list.h"
#include "containers/iterator.h"
#include "macros.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

array_list_init(int);
struct foo {
  int num;
};

typedef struct foo foo;

array_list_init(foo);

void foo_add_one(const void *const current_element, const void *result) {
  const struct foo *const elem = current_element;
  struct foo *elem_copy = map_copy(*elem);
  elem_copy->num++;
  send_to(*elem_copy, result);
}

uint8_t foo_is_even(const void *const current_element) {
  const struct foo *const elem = current_element;
  if (elem->num % 2 == 0) {
    return 1;
  }
  return 0;
}

int main() {
  struct array_list_int *al = array_list_create_int();
  assert(null != al);
  array_list_append_int(al, 1);
  array_list_append_int(al, 2);
  array_list_append_int(al, 3);
  assert(3 == *array_list_get_int_at(al, 2));

  struct array_list_foo *al2 = array_list_create_foo();
  assert(null != al2);
  struct foo f = {.num = 1};
  array_list_append_foo(al2, f);
  f.num = 2;
  array_list_append_foo(al2, f);
  printf("at 1 %d\n", array_list_get_foo_at(al2, 1)->num);
  f.num = 3;
  array_list_append_foo(al2, f);

  for (struct iterator *it = iterator_begin(al2);
       iterator_element(it) != iterator_end(it); iterator_increment(it)) {
    printf("before mutation: %d\n", iterator_element_foo(it)->num);
  }

  struct foo result = {};

  for (struct iterator_mutating *itm =
           iterator_map(iterator_begin(al2), foo_add_one, &result);
       null != itm &&
       iterator_mutating_end(itm) != iterator_mutating_element(itm);
       iterator_mutating_increment(itm)) {
    printf("with foo_add_one: %d\n",
           (cast(struct foo *, iterator_mutating_element(itm)))->num);
  }

  for (struct iterator_mutating *itm =
           iterator_filter(iterator_begin(al2), foo_is_even);
       null != itm &&
       iterator_mutating_end(itm) != iterator_mutating_element(itm);
       iterator_mutating_increment(itm)) {
    printf("foo is even: %d\n",
           (cast(struct foo *, iterator_mutating_element(itm)))->num);
  }

  struct itertator *it = iterator_begin(al2);
  for (struct iterator_mutating *itm = iterator_mutating_filter(
           iterator_mutating_map(iterator_map(it, foo_add_one, &result),
                                 foo_add_one, &result),
           foo_is_even);
       null != itm &&
       iterator_mutating_end(itm) != iterator_mutating_element(itm);
       iterator_mutating_increment(itm)) {
    printf("foo is even after plus 2: %d\n",
           (cast(struct foo *, iterator_mutating_element(itm)))->num);
  }
}
