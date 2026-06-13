#include "grammar.h"
#include "arena.h"
#include "token.h"

#include "containers/stack.h"
#include "containers/string.h"
#include "containers/string_builder.h"

struct _ge {
  stack__parser_state *_context;
};

struct _ge *_validator_create() {
  struct _ge *ge = aalloc(sizeof(struct _ge));
  if (ge == null)
    return null;
  stack__parser_state *context = stack_create__parser_state();
  if (context == null)
    return null;
  ge->_context = context;
  return ge;
}

void new_context(struct _ge *ge, _parser_state ctx) {
  stack_push__parser_state(ge->_context, ctx);
}

void end_context(struct _ge *ge) { stack_pop__parser_state(ge->_context); }

string *invalid_token_error(struct token *token, const char *const append) {
  return string_builder_build(string_builder_join(
      string_builder_join(
          string_builder_join(
              string_builder_join(string_builder_create(), "Invalid token "),
              token_to_string(token)),
          ", "),
      append));
}

_grammar_result *result_is_ok(_grammar_result *result, struct _ge *ge,
                              enum state state) {
  result->ok = 1;
  result->state = state;
  new_context(ge, state);
  return result;
}

_grammar_result *result_is_ok_last_state(_grammar_result *result,
                                         struct _ge *ge) {
  result->ok = 1;
  result->state = stack_size(ge->_context) == 0
                      ? end
                      : *stack_pop__parser_state(ge->_context);
  return result;
}

_grammar_result *result_is_not_ok(_grammar_result *result,
                                  string *diagnostics) {
  result->ok = 0;
  result->diagnostic = diagnostics;
  return result;
}

typedef enum token_type token_type;
array_list_init(token_type);

_grammar_result *validate_token(struct _ge *ge, struct token *token) {
  _grammar_result *result = aalloc(sizeof(_grammar_result));
  if (result == null)
    return null;
  typedef stack__parser_state *context;
  context ctx = ge->_context;
  if (stack_size(ctx) == 0) {
    if (token->type == left_square_bracket) {
      return result_is_ok(result, ge, in_list);
    } else if (token->type == left_brace) {
      return result_is_ok(result, ge, in_object);
    }
    return result_is_not_ok(
        result, invalid_token_error(
                    token, "expected '{' or '[' at the start of the document"));
  }
  _parser_state current_state = *stack_pop__parser_state(ctx);
  // we expect no whitespaces here
  switch (current_state) {
  case in_object:
    // need a key, logical next state is in_key transition or end of the object
    switch (token->type) {
    case quote:
      return result_is_ok(result, ge, in_key);
    case right_brace:
      end_context(ge);
      printf("here\n");
      return result_is_ok_last_state(result, ge);
    default:
      return result_is_not_ok(
          result, invalid_token_error(token, "expected start of key '\"'"));
    }
  case in_list:
    // almost anything except for a delimiter
  case in_key:
  case in_value:
  case delim:
    break;
  }
  return result_is_not_ok(
      result, string_create("expected '{' or '[' for start of document"));
}
