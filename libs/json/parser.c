#include "arena.h"
#include "containers/array_list.h"
#include "containers/iterator.h"
#include "containers/string.h"
#include "containers/string_builder.h"
#include "json.h"
#include "token.h"

#define to_base_array_list(al) cast(struct array_list *, al)

struct token *next_non_empty_token(struct iterator *it) {
  struct token *token = null;
  while (iterator_end(it) != iterator_element(it)) {
    token = iterator_element_token(it);
    iterator_increment(it);

    if (token->type == space)
      continue;
  }
  return token;
}

string *parse_key(struct iterator *it) { return null; }

struct json_value *parse_string(struct iterator *it) {
  string_builder *sb = string_builder_create();
  for_each_it(it) {
    struct token *token = iterator_element_token(it);
    if (token->type == quote) {
      break;
    }
    switch (token->type) {
    case words:
      string_builder_join(sb, token->value);
      break;
    case space:
      string_builder_join(sb, " ");
      break;
    default:
      fprintf(stderr, "expected character stream, got '%s'\n",
              token_to_string(token));
      return null;
    }
  }
  string *str = string_builder_build(sb);
  struct json_value *jv = aalloc(sizeof(struct json_value));
  if (jv == null) {
    return null;
  }
  jv->kind = json_string;
  jv->string = str;
  return jv;
}

struct json_value *parse_list(struct iterator *it) { return null; }

struct json_value *parse_object(struct iterator *it) { return null; }

struct json_value *parse(struct array_list_token *al) {
  for_each(it, to_base_array_list(al)) {
    struct token *token = iterator_element_token(it);
  }
  return null;
}
