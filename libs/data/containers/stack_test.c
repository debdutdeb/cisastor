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

IT(should_peek_pop_correctly) {
  stack_int *s = stack_create_int();
  stack_push_int(s, 0);
  int *i = null;
  i = stack_peek_int(s);
  cassert(i != null, "peek should not be null, value should be 0 at the "
                     "beginning that is peeked into");
  cassert(*i == 0, "should peek value 0");
  stack_push_int(s, 1);
  i = stack_peek_int(s);
  cassert(i != null,
          "peek should not be null, value should be 1 that is peeked into");
  cassert(*i == 1, "peek should be 1");
  i = stack_pop_int(s);
  cassert(i != null,
          "pop should not be null, value should be 1 that is popped");
  cassert(*i == 1, "pop should be 1");
  i = stack_peek_int(s);
  cassert(i != null,
          "peek should not be null, value should be 0 that is peeked into");
  cassert(*i == 0, "peek should be 0");
}
