#include "testing.h"

#include "grammar.h"
#include "token.h"

struct _ge *_validator_create();

static struct token tok(enum token_type type) {
  struct token tok = {.type = type};
  return tok;
}

IT(should_reject_right_brace_at_start) {
  struct token token = tok(right_brace);

  _grammar_result *r = validate_token(_validator_create(), &token);

  cassert(!r->ok, "expected right brace to be invalid for json start");
}

IT(should_reject_comma_at_start) {
  struct token token = tok(comma);

  _grammar_result *r = validate_token(_validator_create(), &token);

  cassert(!r->ok, "expected comma to be invalid for json start");
}

IT(should_reject_quote_at_start) {
  struct token token = tok(quote);

  _grammar_result *r = validate_token(_validator_create(), &token);

  cassert(!r->ok, "expected quote to be invalid for json start");
}

IT(should_reject_words_at_start) {
  struct token token = tok(words);

  _grammar_result *r = validate_token(_validator_create(), &token);

  cassert(!r->ok, "expected words token to be invalid for json start");
}

IT(should_reject_number_at_start) {
  struct token token = tok(number);

  _grammar_result *r = validate_token(_validator_create(), &token);

  cassert(!r->ok, "expected number token to be invalid for json start");
}

IT(should_reject_boolean_at_start) {
  struct token token = tok(boolean);

  _grammar_result *r = validate_token(_validator_create(), &token);

  cassert(!r->ok, "expected boolean token to be invalid for json start");
}

IT(should_reject_nullish_at_start) {
  struct token token = tok(nullish);

  _grammar_result *r = validate_token(_validator_create(), &token);

  cassert(!r->ok, "expected null token to be invalid for json start");
}

IT(should_reject_right_square_bracket_at_start) {
  struct token token = tok(right_square_bracket);

  _grammar_result *r = validate_token(_validator_create(), &token);

  cassert(!r->ok, "expected right square bracket to be invalid for json start");
}
IT(should_enter_object_state_after_left_brace) {
  struct token token = tok(left_brace);

  _grammar_result *r = validate_token(_validator_create(), &token);

  cassert(r->ok, "expected left brace to be accepted");
  cassertf(r->state == in_object, "expected state %d got %d", in_object,
           r->state);
}

IT(should_enter_list_state_after_left_square_bracket) {
  struct token token = tok(left_square_bracket);

  _grammar_result *r = validate_token(_validator_create(), &token);

  cassert(r->ok, "expected left square bracket to be accepted");
  cassertf(r->state == in_list, "expected state %d got %d", in_list, r->state);
}
IT(should_accept_empty_object) {
  cassert(
      validate_token(_validator_create(), &(struct token){.type = left_brace})
          ->ok,
      "expected object start");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = right_brace})
          ->ok,
      "expected object end");
}

IT(should_accept_empty_list) {
  cassert(validate_token(_validator_create(),
                         &(struct token){.type = left_square_bracket})
              ->ok,
          "expected list start");

  cassert(validate_token(_validator_create(),
                         &(struct token){.type = right_square_bracket})
              ->ok,
          "expected list end");
}

IT(should_accept_key_after_object_start) {
  cassert(
      validate_token(_validator_create(), &(struct token){.type = left_brace})
          ->ok,
      "expected object start");

  _grammar_result *r =
      validate_token(_validator_create(), &(struct token){.type = quote});

  cassert(r->ok, "expected quote to begin object key");
}

IT(should_reject_colon_immediately_after_object_start) {
  cassert(
      validate_token(_validator_create(), &(struct token){.type = left_brace})
          ->ok,
      "expected object start");

  _grammar_result *r =
      validate_token(_validator_create(), &(struct token){.type = colon});

  cassert(!r->ok, "expected colon without key to be invalid");
}

IT(should_reject_eof_at_start) {
  struct token token = tok(eof);

  _grammar_result *r = validate_token(_validator_create(), &token);

  cassert(!r->ok, "expected eof to be invalid at start of document");
}

IT(should_accept_colon_after_key) {
  cassert(
      validate_token(_validator_create(), &(struct token){.type = left_brace})
          ->ok,
      "expected object start");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = quote})->ok,
      "expected key");

  _grammar_result *r =
      validate_token(_validator_create(), &(struct token){.type = colon});

  cassert(r->ok, "expected colon after key");
}
IT(should_accept_words_after_colon) {
  cassert(
      validate_token(_validator_create(), &(struct token){.type = left_brace})
          ->ok,
      "expected object start");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = quote})->ok,
      "expected key");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = colon})->ok,
      "expected colon");

  _grammar_result *r =
      validate_token(_validator_create(), &(struct token){.type = words});

  cassert(r->ok, "expected words token after colon");
}
IT(should_accept_comma_after_value) {
  cassert(
      validate_token(_validator_create(), &(struct token){.type = left_brace})
          ->ok,
      "expected object start");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = quote})->ok,
      "expected key");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = colon})->ok,
      "expected colon");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = words})->ok,
      "expected value");

  _grammar_result *r =
      validate_token(_validator_create(), &(struct token){.type = comma});

  cassert(r->ok, "expected comma after value");
}
IT(should_accept_right_brace_after_value) {
  cassert(
      validate_token(_validator_create(), &(struct token){.type = left_brace})
          ->ok,
      "expected object start");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = quote})->ok,
      "expected key");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = colon})->ok,
      "expected colon");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = words})->ok,
      "expected value");

  _grammar_result *r =
      validate_token(_validator_create(), &(struct token){.type = right_brace});

  cassert(r->ok, "expected object close after value");
}
IT(should_accept_second_key_after_comma) {
  cassert(
      validate_token(_validator_create(), &(struct token){.type = left_brace})
          ->ok,
      "expected object start");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = quote})->ok,
      "expected key");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = colon})->ok,
      "expected colon");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = words})->ok,
      "expected value");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = comma})->ok,
      "expected comma");

  _grammar_result *r =
      validate_token(_validator_create(), &(struct token){.type = quote});

  cassert(r->ok, "expected second key after comma");
}
IT(should_reject_double_colon) {
  cassert(
      validate_token(_validator_create(), &(struct token){.type = left_brace})
          ->ok,
      "expected object start");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = quote})->ok,
      "expected key");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = colon})->ok,
      "expected colon");

  _grammar_result *r =
      validate_token(_validator_create(), &(struct token){.type = colon});

  cassert(!r->ok, "expected second colon to be invalid");
}
IT(should_reject_double_comma) {
  cassert(
      validate_token(_validator_create(), &(struct token){.type = left_brace})
          ->ok,
      "expected object start");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = quote})->ok,
      "expected key");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = colon})->ok,
      "expected colon");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = words})->ok,
      "expected value");

  cassert(
      validate_token(_validator_create(), &(struct token){.type = comma})->ok,
      "expected comma");

  _grammar_result *r =
      validate_token(_validator_create(), &(struct token){.type = comma});

  cassert(!r->ok, "expected second comma to be invalid");
}

IT(should_validate_empty_objects) {
  struct _ge *ge = _validator_create();
  cassert(validate_token(ge, &cast(struct token, {.type = left_brace}))->ok,
          "left brace should be valid document start");
  cassert(validate_token(ge, &cast(struct token, {.type = right_brace}))->ok,
          "right brace should be valid document end");
}
