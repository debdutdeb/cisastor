#include "containers/stack.h"
#include "containers/string.h"

#include "token.h"

// what is a grammar?
// It tells what is valid through context, context that is attached to the word
// befire and after. But from a token's perspective, just the previous and next
// token's context can validate a grammar incorrectly. For example, `":"` is
// valid, but if these three tokens are like `{"key"":"`, then grammar is
// invalid. So, essentially, current and next token's are not enough. We need
// another piece of context that is an accumulation of all the contexts before.

// state of a grammar, think about accumulating multiple contexts.
// transition of this state is controlled and validated according to the
// language grammar
typedef enum state {
  begin,

  in_object,
  in_list,

  in_kv_pair,

  in_key,
  in_kv_delimiter,
  in_value,
  in_kv_pair_delimiter,

  end,

  _state_types_count,
} _parser_state;

// in_object, in_kv_pair, in_key
// in_object, in_kv_pair, in_kv_delimiter
// in_object, in_kv_pair, in_value
// in_object, in_kv_pair, in_kv_pair_delimiter
// ...
// [in_kv_pair_delimiter][comma] = TO(1, state_pop, null),
// [in_kv_pair_delimiter][left_brace] = TO(1, {state_pop, state_pop, end}, null}
// [in_kv_pair_delimiter][left_square_bracket] = TO(1, {state_pop, state_pop,
// end}, null}
// so, we need a recursive rule validation
// [in_kv_pair_delimiter][left_brace] = TO(1, state_pop, null}, // in_object,
// in_kv_pair
// [in_kv_pair][left_brace] = TO(1, state_pop, null}, // in_object
// [in_object][left_brace] = TO(1, state_pop, null), // {begin} // the shadow
// state
// [begin][left_brace] = TO(1, end, null), // {begin,end} grammar is done
// [end][default_token_entry_index] = TO(0, state_invalid /* same as token
//     invalid */, "invalid token detected after json document ended"),

typedef struct {
  uint8_t ok;
  union {
    _parser_state state;
    string *diagnostic;
  };
} _grammar_result;

// the "accumulation" is approached through a stack.
// One context can be shadowed by another, as far as the current cursor for the
// tokens is considered.
// the stack itself describes a subset of the language grammar pretty well, for
// example {in_key, in_object} is not a valid state, given the state should
// always start with either in_object or in_list (for ours at least). In other
// words, our state stack willfully describes the spec of the grammar the parser
// should and does support.
stack_init(_parser_state);

// I'd prefer to not leak the state stack,
// and, especially not have a global state stack when a parsing can happen
// multiple times for multiple objects; Not that thread safety is a major
// concern yet, nor this change will make parsing thrread safe. More importantly
// i don't know yet. For the time being, what I know is that global state for
// parsing == bad, local state == helpful for later thraed safety audit.
struct _ge;

// for_each(it, tokens) {
//  struct token* token = next_non_empty_token(it);
//  string* diag = validate_token(token);
//  if (diag != null) {
//    fprintf(stderr, "Invalid token \"%s\": %s\n", string_to_primitive(diag));
//    return null;
//  }
//  // token validated, now we need to know what to do about this token
// }
//
// We could have an "action" which dicated what to do with the token, but that's
// hard to quantify for all types of grammars. Instead, we expose the states
// directly to caller as part of the result struct;
//
// _grammar_result * result = validate_token(token);
// if (!result->ok) {
//  fprintf(stderr, "Invalid token \"%s\": %s\n",
//  string_to_primitive(result->diagnostic)); return null;
// }
//
// At any point we need to validate the current token against the current
// accumulated global context, which should complete the "lookback" portion of
// the stream.
// What we must ensure is HOW we process the token. In other words seperating
// the "grammar" from what the "parser" will do. Grammar either allows or
// disallows, enough diagnostics to tell the user why disallowed. parser knows
// how to accumulate the tokens into the final struct.
_grammar_result *validate_token(struct _ge *, struct token *token);
// ^ returns a diagnostic message, null if is allowed

/*
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
 * ----
 * Looking at the code above, it's a table, like a bool table.
 * row = current_state
 * column = token->type
 * Instead of a imperative switches and ifs, a declarative way of defining and
 using a table makes more sense here.
 * Likely a container type.
 * Yes, hashmap with a tuple key is essentially a table.
 * No, simple 2d table
 *
*/
