#include "arena.h"
#include "containers/array_list.h"
#include "containers/iterator.h"
#include "containers/string.h"
#include "json.h"
#include "token.h"

#define to_base_array_list(al) cast(struct array_list *, al)

// All parxe_* functions expect the iterator to be in the next position,
// avoiding double checks. E.g. parse_object does not expect the current token
// to be '{', rather a skipped whitespace and then '"' for the start of the key.

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

enum state {
  expecting_key,
};

enum state state = expecting_key;

string *parse_key(struct iterator *it) { return null; }

struct json_value *parse_string(struct iterator *it) { return null; }

struct json_value *parse_list(struct iterator *it) { return null; }

struct json_value *parse_object(struct iterator *it) {
  struct token *token = next_non_empty_token(it);
  if (token->type != quote) {
    fprintf(stderr, "Expected start of field key, expected '\"', got '%s'\n",
            token_to_string(token));
    return null;
  }
  // consume token
  iterator_increment(it);
  string *key = parse_key(it);
  if (key == null) {
    fprintf(stderr, "Failed to parse json key\n");
    return null;
  }
  token = next_non_empty_token(it);
  if (token->type != colon) {
    fprintf(stderr, "Expected ':'. got '%s'\n", token_to_string(token));
    return null;
  }
  token = next_non_empty_token(it);
  switch (token->type) {
  case quote: {
    struct json_value *value = parse_string(it);
    break;
  }
  case left_brace: {
    struct json_value *value = parse_object(it);
    break;
  }
  case left_square_bracket: {
    struct json_value *value = parse_list(it);
    break;
  }
  case nullish:
  case number:
  default:
    fprintf(stderr, "Unknown token '%s'\n", token_to_string(token));
    return null;
  }
  return null;
}

struct json_value *parse(struct array_list_token *al) {
  for_each(it, to_base_array_list(al)) {
    struct token *token = iterator_element_token(it);
    switch (token->type) {
    case left_brace:
      return parse_object(it);
    case left_square_bracket:
      return parse_list(it);
    default:
      fprintf(stderr, "Invalid token %s\n", token_to_string(token));
    }
  }
  return null;
}
