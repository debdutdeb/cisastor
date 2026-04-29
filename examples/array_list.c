#include "containers/array_list.h"
#include "containers/iterator.h"
#include "macros.h"

#include <assert.h>
#include <stdio.h>

array_list_init(int);

struct foo {
  int num;
};

typedef struct foo foo;

array_list_init(foo);

void *plus_one(void *element) {
  struct foo *foo_element = cast(struct foo *, element);
  foo_element->num++;
  return foo_element;
}

uint8_t is_even(void *element) {
  struct foo *foo_element = cast(struct foo *, element);
  if (foo_element->num % 2 == 0) {
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
  f.num = 3;
  array_list_append_foo(al2, f);

  for (struct iterator *it = iterator_begin(al2);
       iterator_element(it) != iterator_end(it); iterator_increment(it)) {
    printf("before transformation: %d\n", iterator_element_foo(it)->num);
  }

  /* for (struct iterator *mapped_it = iterator_map(iterator_begin(al2),
   * plus_one); */
  /*      iterator_current(mapped_it) != iterator_end(mapped_it); */
  /*      iterator_increment(mapped_it)) { */
  /*   struct foo *transformed = iterator_element_foo(mapped_it); */
  /*   // no null for map */
  /*   printf("transformed by map: %d\n", transformed->num); */
  /* } */

  for (struct iterator *mapped_it = iterator_filter(
           iterator_map(iterator_begin(al2), plus_one), is_even);
       iterator_element(mapped_it) != iterator_end(mapped_it);
       iterator_increment(mapped_it)) {
    struct foo *transformed = iterator_element_foo(mapped_it);
    printf("transformed by map, but ensured it is even: %d\n",
           transformed->num);
  }

  for (struct iterator *mapped_it = iterator_map(
           iterator_filter(iterator_begin(al2), is_even), plus_one);
       iterator_element(mapped_it) != iterator_end(mapped_it);
       iterator_increment(mapped_it)) {
    struct foo *transformed = iterator_element_foo(mapped_it);
    printf("filtered if even before transforming: %d\n", transformed->num);
  }
}
