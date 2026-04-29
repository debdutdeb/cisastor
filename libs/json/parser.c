#include "arena.h"
#include "containers/array_list.h"
#include "containers/iterator.h"
#include "json.h"
#include "token.h"

typedef struct token token;

array_list_init(token);

int is_whitespace(struct token *tok) {
  enum token_type t = tok->type;
  return space == t || newline == t;
}

enum parser_state {
  state_expect_object_start,
  state_expect_key_start,
  state_expect_key_value_seperator,
  state_expect_value_start,
  state_expect_list_end,
  state_expect_object_end,
};

void consume_whitespace(struct iterator *it) {
  for (; iterator_end(it) != iterator_element(it); iterator_increment(it)) {
    struct token *token = iterator_element_token(it);
    switch (token->type) {
    case space:
    case newline:
      continue;
    default:
      return;
    }
  }
}

struct json_object *parse_object(struct iterator *it) { return null; }

struct json_list *parse_list(struct iterator *it) { return null; }

struct json_value *parse_primitive(struct iterator *it) {
  struct token *current_token = iterator_element_token(it);
  switch (current_token->type) {
  case quote:
    for (iterator_increment(it); iterator_end(it) != iterator_element(it);
         iterator_increment(it)) {
      // 
    }

  default:
    break;
  }
  return null;
}

struct json_value *parse(struct array_list_token *token_list) {
  struct json_value *root = aalloc(sizeof(struct json_value));
  if (null == root) {
    return null;
  }

  enum parser_state state = state_expect_object_start;

  struct iterator *it = iterator_begin(token_list);
  consume_whitespace(it);
  struct token *tok = iterator_element_token(it);
  switch (tok->type) {
  case left_brace:
    root->kind = json_object;
    root->object = parse_object(it);
  case left_square_bracket:
    root->kind = json_list;
    root->list = parse_list(it);
  default:
    fprintf(stderr, "expected start of json ('{' or '[') got '%s'\n",
            token_to_string(tok));
    return null;
  }
  return root;
}
