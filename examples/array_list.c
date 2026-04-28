#include "../data/containers/array_list.h"
#include "macros.h"

#include <assert.h>
#include <stdio.h>

array_list_init(int);

struct foo {
  int num;
};

typedef struct foo foo;

array_list_init(foo);

int main() {
  struct array_list *al = array_list_create_int();
  assert(null != al);
  array_list_append_int(al, 1);
  array_list_append_int(al, 2);
  array_list_append_int(al, 3);
  assert(3 == array_list_get_int_at(al, 2));

  struct array_list *al2 = array_list_create_foo();
  assert(null != al2);
  struct foo f = {.num = 1};
  array_list_append_foo(al2, f);
  f.num = 2;
  array_list_append_foo(al2, f);
  printf("at 1 %d\n", array_list_get_foo_at(al2, 1).num);
}
