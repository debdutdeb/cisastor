#include "string_builder.h"
#include "testing.h"

IT(should_create_string_builder) {
  string_builder *sb = string_builder_create();
  cassert(sb != NULL, "failed to create string_builder");
}

IT(should_join_strings) {
  string_builder *sb = string_builder_create();
  string_builder_join(sb, "hello ");
  string_builder_join(sb, "world");

  string *str = string_builder_build(sb);
  cassert(string_length(str) == 11, "length should be 11");

  string *expected = string_create("hello world");
  cassert(string_cmp(str, expected) == 0, "string mismatch");
}

IT(should_handle_many_joins) {
  string_builder *sb = string_builder_create();
  for (int i = 0; i < 1000; i++) {
    string_builder_join(sb, "a");
  }

  string *str = string_builder_build(sb);
  cassert(string_length(str) == 1000, "length should be 1000");
  for (int i = 0; i < 1000; i++) {
    cassertf(string_char_at(str, i) == 'a', "mismatch at idx %d", i);
  }
}

