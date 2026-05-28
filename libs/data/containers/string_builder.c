#include "string_builder.h"
#include "array_list.h"
#include "string.h"

#include <string.h>

string_builder *string_builder_create() {
  struct array_list_const_char_ptr *al = array_list_create_const_char_ptr();
  return al;
}

string_builder *string_builder_join(string_builder *sb,
                                    const char *const cstr) {
  array_list_append_const_char_ptr(cast(struct array_list_const_char_ptr *, sb),
                                   cstr);
  return sb;
}

string *string_builder_build(string_builder *sb) {
  size_t capacity = 0;
  for (int i = 0; i < array_list_get_length(sb); i++) {
    capacity += strlen(*array_list_get_const_char_ptr_at(sb, i));
  }
  string *str = cast(string *, array_list_create_char_with_capacity(capacity));
  for (int i = 0; i < array_list_get_length(sb); i++) {
    const char *cstr = *array_list_get_const_char_ptr_at(sb, i);
    for (int j = 0; cstr[j] != '\0'; j++) {
      string_append_char(str, cstr[j]);
    }
  }
  return str;
}
