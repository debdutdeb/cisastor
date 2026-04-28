#include "array_list.h"
// Use of arena allocator is not good for something growing.
// Or rather, use of an arena allocator implementation such as this isn't ideal,
// when reallocation is needed. We'll be left with a lot of fragmentation and
// empty spaces. But, fine for now. As arena allocator adds reallocation
// capability and defrags, this will get better; not to mention allowing custom
// allocators later on.
#include "arena.h"
#include "macros.h"

const size_t default_capacity = 5;

size_t array_list_index_element_progress_by(struct array_list *al, int index) {
  return index * al->element_size;
}

struct array_list *array_list_create(size_t size) {
  return array_list_create_with_capacity(size, default_capacity);
}

struct array_list *array_list_create_with_capacity(size_t size,
                                                   uint32_t capacity) {
  struct array_list *al = aalloc(sizeof(struct array_list));
  if (null == al) {
    return null;
  }
  al->region = aalloc(size * capacity);
  if (null == al->region) {
    return null;
  }
  al->length = 0;
  al->element_size = size;
  al->capacity = capacity;
  return al;
}

byte *array_list_get_element_at(struct array_list *al, int index) {
  if (0 > index) {
    return null;
  }
  if (al->length <= index) {
    return null;
  }
  return al->region + array_list_index_element_progress_by(al, index);
}

byte *array_list_append(struct array_list *al, const byte *data) {
  if (al->capacity == al->length) {
    size_t old_size = al->element_size * al->length;
    byte *old_region = al->region;
    al->region =
        reaalloc(al->region, old_size + default_capacity * al->element_size);
    if (null == al->region) {
      return null;
    }
    al->capacity += default_capacity;
    memcpy(al->region, old_region, old_size);
  }
  size_t progress = array_list_index_element_progress_by(al, al->length);
  memcpy(al->region + progress, data, al->element_size);
  al->length++;
  return al->region + progress;
}
