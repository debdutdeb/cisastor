#include "containers/array_list.h"
#include "containers/iterator.h"
#include "containers/string.h"
#include "json.h"
#include "testing.h"
#include "token.h"
#include <string.h>

static uint8_t string_equals_cstr(string *s, const char *cstr) {
  return strcmp(string_to_primitive_underlying(s), cstr) == 0;
}

IT(should_parse_empty_string) {
  struct array_list_token *al = array_list_create_token();
  token t1 = {.type = quote};
  array_list_append_token(al, t1);

  struct iterator *it = iterator_begin((struct array_list *)al);
  struct json_value *jv = parse_string(it);

  cassert(jv != NULL, "jv != NULL");
  cassert(jv->kind == json_string, "kind == json_string");
  cassert(string_length(jv->string) == 0, "length == 0");
  cassert(iterator_element_token(it)->type == quote, "at quote");
}

IT(should_parse_string_with_words) {
  struct array_list_token *al = array_list_create_token();
  token t1 = {.type = words, .value = "hello"};
  token t2 = {.type = quote};
  array_list_append_token(al, t1);
  array_list_append_token(al, t2);

  struct iterator *it = iterator_begin((struct array_list *)al);
  struct json_value *jv = parse_string(it);

  cassert(jv != NULL, "jv != NULL");
  cassert(string_equals_cstr(jv->string, "hello"), "equals hello");
  cassert(iterator_element_token(it)->type == quote, "at quote");
}

IT(should_parse_string_with_spaces) {
  struct array_list_token *al = array_list_create_token();
  token t1 = {.type = words, .value = "a"};
  token t2 = {.type = space};
  token t3 = {.type = words, .value = "b"};
  token t4 = {.type = quote};
  array_list_append_token(al, t1);
  array_list_append_token(al, t2);
  array_list_append_token(al, t3);
  array_list_append_token(al, t4);

  struct iterator *it = iterator_begin((struct array_list *)al);
  struct json_value *jv = parse_string(it);

  cassert(jv != NULL, "jv != NULL");
  cassert(string_equals_cstr(jv->string, "a b"), "equals 'a b'");
  cassert(iterator_element_token(it)->type == quote, "at quote");
}

IT(should_parse_string_with_only_spaces) {
  struct array_list_token *al = array_list_create_token();
  token t1 = {.type = space};
  token t2 = {.type = space};
  token t3 = {.type = quote};
  array_list_append_token(al, t1);
  array_list_append_token(al, t2);
  array_list_append_token(al, t3);

  struct iterator *it = iterator_begin((struct array_list *)al);
  struct json_value *jv = parse_string(it);

  cassert(jv != NULL, "jv != NULL");
  cassert(string_equals_cstr(jv->string, "  "), "equals '  '");
  cassert(iterator_element_token(it)->type == quote, "at quote");
}

IT(should_parse_string_with_mixed_words_and_spaces) {
  struct array_list_token *al = array_list_create_token();
  token t1 = {.type = space};
  token t2 = {.type = words, .value = "foo"};
  token t3 = {.type = space};
  token t4 = {.type = words, .value = "bar"};
  token t5 = {.type = space};
  token t6 = {.type = quote};
  array_list_append_token(al, t1);
  array_list_append_token(al, t2);
  array_list_append_token(al, t3);
  array_list_append_token(al, t4);
  array_list_append_token(al, t5);
  array_list_append_token(al, t6);

  struct iterator *it = iterator_begin((struct array_list *)al);
  struct json_value *jv = parse_string(it);

  cassert(jv != NULL, "jv != NULL");
  cassert(string_equals_cstr(jv->string, " foo bar "), "equals ' foo bar '");
  cassert(iterator_element_token(it)->type == quote, "at quote");
}
