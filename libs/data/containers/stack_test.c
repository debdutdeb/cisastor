#include "testing.h"

#include "stack.h"

IT(should_enstack_elements_and_pop) {
  stack_int *q = stack_create_int();
  for (int i = 1; i <= 10000; i++) {
    stack_push_int(q, i);
  }
  cassert(stack_size(q) == 10000, "invalid stack size, should be 10000");
  for (int i = 10000; i > 0; i--) {
    int *_got = stack_pop_int(q);
    cassertf(_got != null, "popped null data at index %d\n", i);
    int got = *_got;
    cassertf(got == i, "expected %d got %d\n", i, got);
  }
  cassert(stack_size(q) == 0, "size should be 0 now");
}
