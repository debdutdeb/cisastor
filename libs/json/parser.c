#include "arena.h"
#include "containers/array_list.h"
#include "containers/iterator.h"
#include "containers/stack.h"
#include "containers/string.h"
#include "containers/string_builder.h"
#include "json.h"
#include "token.h"

#define to_base_array_list(al) cast(struct array_list *, al)

struct token *next_non_empty_token(struct iterator *it) {
  for_each_it(it) {
    struct token *token = iterator_element_token(it);
    if (token->type != space) {
      return token;
    }
  }
  return null;
}

// state works through declaring the grammer in stages.
// state a parser is in, is not a single instance of enum.
// It's multiple states, recursive.
// Therefore, our state is a queue_state, advancing which, involves a next state
// and a finish state, which sets current state to the post-pop of the queue.
// what is a grammar?
// It tells what is valid through context, context that is attached to the word
// befire and after. But from a token's perspective, just the previous and next
// token's context can validate a grammar incorrectly. For example, `":"` is
// valid, but if these three tokens are like `{"key"":"`, then grammar is
// invalid. So, essentially, current and next token's are not enough. We need
// another piece of context that is an accumulation of all the contexts before.

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
    case eof:
      fprintf(stderr, "unexpected EOF while reading string");
      return null;

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
    struct token *token = next_non_empty_token(it);
  }
  return null;
}
