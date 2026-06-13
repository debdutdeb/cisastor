#include "stack.h"
#include "array_list.h"

stack *stack_create(size_t size) { return array_list_create(size); }

byte *stack_pop(stack *q) {
  struct array_list *al = cast(struct array_list *, q);
  void *element = array_list_get_element_at(al, al->length - 1);
  if (element == null) {
    return null;
  }
  // array_list does not own anything, so this is fine, we aren't leaking memory
  // by moving the cursor;
  al->length--;
  return element;
}

void stack_push(stack *q, byte *data) { array_list_append(q, data); }

size_t stack_size(stack *q) { return array_list_get_length(q); }
