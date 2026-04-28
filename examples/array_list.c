#include "../utils/array_list.h"
#include "macros.h"

#include <assert.h>

#define array_list_get_int_at(a, b) *cast(int *, array_list_get_element_at(a, b))

int *array_list_append_int(struct array_list *al, int num) {
  return cast(int *, array_list_append(al, &num));
}

int main() {
  struct array_list *al = array_list_create(sizeof(int));
  assert(null != al);
  assert(null != array_list_append_int(al, 1));
  assert(null != array_list_append_int(al, 3));
  assert(null != array_list_append_int(al, 4));
  assert(null != array_list_append_int(al, 5));
  assert(null != array_list_append_int(al, 6));
  assert(null != array_list_append_int(al, 7));
  assert(null != array_list_append_int(al, 8));
  assert(6 == array_list_get_int_at(al, 4));
}
