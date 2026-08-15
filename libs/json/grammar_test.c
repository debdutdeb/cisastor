#include "testing.h"

#include "grammar.h"
#include "token.h"

struct _ge *_validator_create();

static struct token tok(enum token_type type) {
  struct token tok = {.type = type};
  return tok;
}

IT(should_validate_empty_objects) {
  struct _ge *ge = _validator_create();

  cassert(validate_token(ge, &cast(struct token, {.type = left_brace}))->ok,
          "left brace should be valid document start");
  cassert(validate_token(ge, &cast(struct token, {.type = right_brace}))->ok,
          "right brace should be valid document end");
}

IT(should_validate_empty_lists) {
  struct _ge *ge = _validator_create();
  cassert(validate_token(ge, &cast(struct token, {.type = left_square_bracket}))
              ->ok,
          "left square bracket should be valid document start");
  cassert(
      validate_token(ge, &cast(struct token, {.type = right_square_bracket}))
          ->ok,
      "right square bracket should be valid document end");
}

IT(should_validate_flat_json_object_with_all_typed_values) {
  struct _ge *ge = _validator_create();
  struct token token = tok(left_brace);
  _grammar_result *result = validate_token(ge, &token);
  cassert(result->ok, "{ should be valid json start");
  cassert(result->state == in_object, "{ should start in_object context");
  token = tok(quote);
  result = validate_token(ge, &token);
  cassert(result->ok, "{\" should be valid json");
  cassert(result->state == in_key, "{ should start in_key context");
  token = tok(quote);
  result = validate_token(ge, &token);
  cassert(result->ok, "{\"\" should be valid json");
  cassert(result->state == delim, "{\"\" should set context to delim");
  token = tok(colon);
  result = validate_token(ge, &token);
  cassert(result->ok, "{\"\": should be valid json");
  cassert(result->state == in_value, "{\"\": should set context to in_value");
  token = tok(number);
  result = validate_token(ge, &token);
  cassert(result->ok, "{\"\":8 should be valid json");
  cassert(result->state == delim, "{\"\":8 should set context to delim");
  token = tok(comma);
  result = validate_token(ge, &token);
  cassert(result->ok, "{\"\":8, should be valid json");
  cassert(result->state == in_object,
          "{\"\":8, should set context to in_object");
  token = tok(quote);
  validate_token(ge, &token);
  token = tok(quote);
  validate_token(ge, &token);
  token = tok(colon);
  validate_token(ge, &token);
  // {\"\":8,\"\": <- so far
  token = tok(quote);
  result = validate_token(ge, &token);
  cassert(result->ok, "{\"\":8,\"\":\" should be valid json");
  cassert(result->state == in_value,
          "{\"\":8,\"\":\" should set context to in_value");
  token = tok(quote);
  result = validate_token(ge, &token);
  cassert(result->ok, "{\"\":8,\"\":\"\" should be valid json");
  cassert(result->state == in_object,
          "{\"\":8,\"\":\"\" should set context to in_object");
}
