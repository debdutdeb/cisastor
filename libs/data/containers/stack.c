#include "stack.h"
#include "array_list.h"

stack *stack_create(size_t size) { return array_list_create(size); }

byte *stack_peek(stack *s) {
  struct array_list *al = cast(struct array_list *, s);
  void *element = array_list_get_element_at(al, al->length - 1);
  if (element == null) {
    return null;
  }
  return element;
}

byte *stack_pop(stack *s) {
  void *element = stack_peek(s);
  if (element == null) {
    return null;
  }
  // array_list does not own anything, so this is fine, we aren't leaking memory
  // by moving the cursor;
  cast(struct array_list *, s)->length--;
  return element;
}

void stack_push(stack *s, byte *data) { array_list_append(s, data); }

size_t stack_size(stack *s) { return array_list_get_length(s); }
