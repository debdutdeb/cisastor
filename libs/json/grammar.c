#include "grammar.h"
#include "arena.h"
#include "macros.h"
#include "token.h"

#include "containers/stack.h"
#include "containers/string.h"
#include "containers/string_builder.h"

// for unreachable
#include <stdio.h>

const uint8_t token_types_length_plus_one = _token_types_count + 1;
const uint8_t state_types_length_plus_one = _state_types_count + 1;

const uint8_t default_token_entry_index = _token_types_count;
const uint8_t default_state_entry_index = _state_types_count; // unused

typedef enum {
  state_pop = -1,
  state_invalid = -2,
} _grammar_special_transition;

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
  stack_push__parser_state(context, begin);
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

struct _grammar_rule;

typedef _grammar_result *(*_grammar_rule_build_grammar_result_t)(
    struct _grammar_rule *, struct _ge *, struct token *);

struct _grammar_rule {
  _grammar_rule_build_grammar_result_t _build;
  const char *const _diagnostics;
  _grammar_result _template;
};

uint8_t _grammar_rule_is_empty(struct _grammar_rule *rule) {
  return rule->_build == null;
}

_grammar_result *_grammar_rule_build_grammar_result(struct _grammar_rule *_rule,
                                                    struct _ge *_ge,
                                                    struct token *token) {
  if (_rule->_template.ok) {
    if (_rule->_template.state == state_pop) {
      return result_is_ok_last_state(&_rule->_template, _ge);
    }
    return result_is_ok(&_rule->_template, _ge, _rule->_template.state);
  }

  // FIXME uncomment once all tokens comint to this layer is filtered to only
  // what it concerns itself with
  //
  // if (_rule->_template.state != state_invalid) {
  //   unreachable("for .ok == 0, state should be -2");
  // }

  return result_is_not_ok(&_rule->_template,
                          invalid_token_error(token, _rule->_diagnostics));
}

#define TO(is_ok, next, diag)                                                  \
  {                                                                            \
    ._build = &_grammar_rule_build_grammar_result, ._diagnostics = diag,       \
    ._template = {                                                             \
      .ok = is_ok,                                                             \
      .state = next,                                                           \
    }                                                                          \
  }

static struct _grammar_rule _grammar_context_table
    [state_types_length_plus_one][token_types_length_plus_one] = {
        [begin][left_brace] = TO(1, in_object, null),
        [begin][left_square_bracket] = TO(1, in_list, null),
        [begin][default_token_entry_index] = TO(
            0, state_invalid, "expected '{' or '[' as beginning of document"),

        // ---

        [in_object][quote] = TO(1, in_key, null),
        [in_object][right_brace] = TO(1, state_pop, null),
        [in_object][default_token_entry_index] =
            TO(0, state_invalid,
               "expected start of key '\"' or end of object '}'"),

        [in_key][quote] = TO(1, state_pop, null),

        [in_list][right_square_bracket] = TO(1, state_pop, null),
};

typedef enum token_type token_type;
array_list_init(token_type);

struct _grammar_rule *_grammar_rule_get(_parser_state state,
                                        enum token_type token_type) {
  struct _grammar_rule *rule = &_grammar_context_table[state][token_type];
  if (_grammar_rule_is_empty(rule))
    return &_grammar_context_table[state][default_token_entry_index];
  return rule;
}

struct _grammar_rule *
_grammar_rule_get_no_fallback(_parser_state state, enum token_type token_type) {
  struct _grammar_rule *rule = &_grammar_context_table[state][token_type];
  if (_grammar_rule_is_empty(rule))
    return null;
  return rule;
}

_grammar_result *validate_token(struct _ge *ge, struct token *token) {
  typedef stack__parser_state *context;
  context ctx = ge->_context;
  _parser_state current_state = *stack_peek__parser_state(ctx);
  struct _grammar_rule *rule = _grammar_rule_get(current_state, token->type);
  _grammar_result *result = rule->_build(rule, ge, token); // begin, in_object
  for (current_state = *stack_peek__parser_state(ctx),
      rule = _grammar_rule_get_no_fallback(current_state, token->type);
       rule != null; current_state = *stack_peek__parser_state(ctx),
      rule = _grammar_rule_get_no_fallback(current_state, token->type)) {
    result = rule->_build(rule, ge, token);
  }
  return result;
}
