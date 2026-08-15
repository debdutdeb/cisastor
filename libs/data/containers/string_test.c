#include "string.h"
#include "testing.h"
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
  // string_cmp implementation uses strncmp with min length, so "abc" vs "ab"
  // might return 0? Let's test what it actually does.
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

IT(should_return_correct_string_length) {
  string *s1 = string_create("");
  for (size_t i = 1; i <= 10000; i++) {
    string_append_char(s1, i);
  }
  cassert(string_length(s1) == 10000, "length should be 10000");
}

IT(should_return_the_same_for_copy_and_no_copy_primitive_functions) {
  string *s1 = string_create("hello world");
  // using strncmp, see comment under string_to_primitive_underlying
  cassert(strncmp(string_to_primitive_underlying(s1), string_to_primitive(s1),
                  string_length(s1)) == 0,
          "Should be equal to \"hello world\"");
}

IT(should_compare_string_instances_correctly) {
  string *s1 = string_create("hello world");
  string *s2 = string_create("hello world");
  cassert(string_cmp(s1, s2) == 0, "s1 and s2 should be equal");
}

/*
 * The following test is expected to fail, see comment under _underlying
function.

IT(should_be_null_terminated_for_underlying_primitive_function) {
  string *s1 = string_create("hello world");
  size_t length = string_length(s1);
  cassert(string_to_primitive_underlying(s1)[length] == '\0',
          "to_primitive_underlying should end string with null character");
}
*/

IT(should_compare_cstrings_correctly) {
  string *s1 = string_create("hello world");
  cassert(string_cmp_cstr(s1, "hello world") == 0,
          "s1 should be equal to \"hello world\"");
}
