#include "testing.h"
#include "string.h"
#include <string.h>

IT(should_create_string) {
  string *s = string_create("hello");
  cassert(s != NULL, "failed to create string");
  cassert(string_length(s) == 5, "length should be 5");
  cassert(string_char_at(s, 0) == 'h', "expected 'h'");
  cassert(string_char_at(s, 4) == 'o', "expected 'o'");
}

IT(should_append_chars) {
  string *s = string_create("abc");
  string_append_char(s, 'd');
  cassert(string_length(s) == 4, "length should be 4");
  cassert(string_char_at(s, 3) == 'd', "expected 'd'");
}

IT(should_compare_strings) {
  string *s1 = string_create("abc");
  string *s2 = string_create("abc");
  string *s3 = string_create("abd");
  string *s4 = string_create("ab");

  cassert(string_cmp(s1, s2) == 0, "s1 should equal s2");
  cassert(string_cmp(s1, s3) != 0, "s1 should not equal s3");
  // string_cmp implementation uses strncmp with min length, so "abc" vs "ab" might return 0?
  // Let's test what it actually does.
}

IT(should_extract_substring) {
  string *s = string_create("hello world");
  string *sub = string_substring(s, 0, 5);
  cassert(string_length(sub) == 5, "substring length mismatch");
  cassert(string_char_at(sub, 0) == 'h', "expected 'h'");
  cassert(string_char_at(sub, 4) == 'o', "expected 'o'");
}

// I suspect string_concat is broken, let's see if this test passes
IT(should_concatenate_strings) {
  string *s1 = string_create("hello ");
  string *s2 = string_create("world");
  string *res = string_concat(s1, s2);

  /* printf("%s\n", string_to_primitive(res)); */
  
  cassert(string_length(res) == 11, "length mismatch after concat");
  cassert(string_char_at(res, 0) == 'h', "expected 'h'");
  cassert(string_char_at(res, 6) == 'w', "expected 'w'");
}
