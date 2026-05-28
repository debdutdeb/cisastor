#define CISASTOR_TESTING_HALT_ON_FAIL 1
#include "testing.h"
#include "token.h"
#include "containers/array_list.h"
#include "containers/iterator.h"
#include <string.h>

typedef struct token token;
array_list_init(token);

TEST(should_break_down_to_tokens) {
  char *json = "{ \"id\": 1, \"active\": true, \"scores\": [10, 20], \"meta\": { \"verified\": null } }";
  struct array_list_token *tokens = tokens_from_json_string(json);
  cassert(tokens != NULL, "failed to tokenize json string");

  struct iterator *it = iterator_begin((struct array_list *)tokens);
  
  // {
  cassert(iterator_element_token(it)->type == left_brace, "expected {");
  iterator_increment(it);

  // space
  cassert(iterator_element_token(it)->type == space, "expected space");
  iterator_increment(it);

  // "
  cassert(iterator_element_token(it)->type == quote, "expected \"");
  iterator_increment(it);

  // id
  cassert(iterator_element_token(it)->type == words, "expected words 'id'");
  cassert(strcmp(token_get_words(iterator_element_token(it)), "id") == 0, "expected 'id'");
  iterator_increment(it);

  // "
  cassert(iterator_element_token(it)->type == quote, "expected \"");
  iterator_increment(it);

  // :
  cassert(iterator_element_token(it)->type == colon, "expected :");
  iterator_increment(it);

  // space
  cassert(iterator_element_token(it)->type == space, "expected space");
  iterator_increment(it);

  // 1
  cassert(iterator_element_token(it)->type == number, "expected number 1");
  cassert(token_get_number(iterator_element_token(it)) == 1, "expected 1");
  iterator_increment(it);

  // ,
  cassert(iterator_element_token(it)->type == comma, "expected ,");
  iterator_increment(it);

  // space
  cassert(iterator_element_token(it)->type == space, "expected space");
  iterator_increment(it);

  // "
  cassert(iterator_element_token(it)->type == quote, "expected \"");
  iterator_increment(it);

  // active
  cassert(iterator_element_token(it)->type == words, "expected words 'active'");
  cassert(strcmp(token_get_words(iterator_element_token(it)), "active") == 0, "expected 'active'");
  iterator_increment(it);

  // "
  cassert(iterator_element_token(it)->type == quote, "expected \"");
  iterator_increment(it);

  // :
  cassert(iterator_element_token(it)->type == colon, "expected :");
  iterator_increment(it);

  // space
  cassert(iterator_element_token(it)->type == space, "expected space");
  iterator_increment(it);

  // true
  cassert(iterator_element_token(it)->type == boolean, "expected boolean true");
  cassert(token_get_bool(iterator_element_token(it)) == 1, "expected true");
  iterator_increment(it);

  // ,
  cassert(iterator_element_token(it)->type == comma, "expected ,");
  iterator_increment(it);

  // space
  cassert(iterator_element_token(it)->type == space, "expected space");
  iterator_increment(it);

  // "
  cassert(iterator_element_token(it)->type == quote, "expected \"");
  iterator_increment(it);

  // scores
  cassert(iterator_element_token(it)->type == words, "expected words 'scores'");
  cassert(strcmp(token_get_words(iterator_element_token(it)), "scores") == 0, "expected 'scores'");
  iterator_increment(it);

  // "
  cassert(iterator_element_token(it)->type == quote, "expected \"");
  iterator_increment(it);

  // :
  cassert(iterator_element_token(it)->type == colon, "expected :");
  iterator_increment(it);

  // space
  cassert(iterator_element_token(it)->type == space, "expected space");
  iterator_increment(it);

  // [
  cassert(iterator_element_token(it)->type == left_square_bracket, "expected [");
  iterator_increment(it);

  // 10
  cassert(iterator_element_token(it)->type == number, "expected number 10");
  cassert(token_get_number(iterator_element_token(it)) == 10, "expected 10");
  iterator_increment(it);

  // ,
  cassert(iterator_element_token(it)->type == comma, "expected ,");
  iterator_increment(it);

  // space
  cassert(iterator_element_token(it)->type == space, "expected space");
  iterator_increment(it);

  // 20
  cassert(iterator_element_token(it)->type == number, "expected number 20");
  cassert(token_get_number(iterator_element_token(it)) == 20, "expected 20");
  iterator_increment(it);

  // ]
  cassert(iterator_element_token(it)->type == right_square_bracket, "expected ]");
  iterator_increment(it);

  // ,
  cassert(iterator_element_token(it)->type == comma, "expected ,");
  iterator_increment(it);

  // space
  cassert(iterator_element_token(it)->type == space, "expected space");
  iterator_increment(it);

  // "
  cassert(iterator_element_token(it)->type == quote, "expected \"");
  iterator_increment(it);

  // meta
  cassert(iterator_element_token(it)->type == words, "expected words 'meta'");
  cassert(strcmp(token_get_words(iterator_element_token(it)), "meta") == 0, "expected 'meta'");
  iterator_increment(it);

  // "
  cassert(iterator_element_token(it)->type == quote, "expected \"");
  iterator_increment(it);

  // :
  cassert(iterator_element_token(it)->type == colon, "expected :");
  iterator_increment(it);

  // space
  cassert(iterator_element_token(it)->type == space, "expected space");
  iterator_increment(it);

  // {
  cassert(iterator_element_token(it)->type == left_brace, "expected {");
  iterator_increment(it);

  // space
  cassert(iterator_element_token(it)->type == space, "expected space");
  iterator_increment(it);

  // "
  cassert(iterator_element_token(it)->type == quote, "expected \"");
  iterator_increment(it);

  // verified
  cassert(iterator_element_token(it)->type == words, "expected words 'verified'");
  cassert(strcmp(token_get_words(iterator_element_token(it)), "verified") == 0, "expected 'verified'");
  iterator_increment(it);

  // "
  cassert(iterator_element_token(it)->type == quote, "expected \"");
  iterator_increment(it);

  // :
  cassert(iterator_element_token(it)->type == colon, "expected :");
  iterator_increment(it);

  // space
  cassert(iterator_element_token(it)->type == space, "expected space");
  iterator_increment(it);

  // null
  cassert(iterator_element_token(it)->type == nullish, "expected null");
  iterator_increment(it);

  // space
  cassert(iterator_element_token(it)->type == space, "expected space");
  iterator_increment(it);

  // }
  cassert(iterator_element_token(it)->type == right_brace, "expected }");
  iterator_increment(it);

  // space
  cassert(iterator_element_token(it)->type == space, "expected space");
  iterator_increment(it);

  // }
  cassert(iterator_element_token(it)->type == right_brace, "expected }");
  iterator_increment(it);

  cassert(iterator_element(it) == iterator_end(it), "expected end of tokens");
}

TEST(should_handle_empty_object) {
  char *json = "{}";
  struct array_list_token *tokens = tokens_from_json_string(json);
  cassert(tokens != NULL, "failed to tokenize empty object");
  struct iterator *it = iterator_begin((struct array_list *)tokens);
  cassert(iterator_element_token(it)->type == left_brace, "expected {");
  iterator_increment(it);
  cassert(iterator_element_token(it)->type == right_brace, "expected }");
  iterator_increment(it);
  cassert(iterator_element(it) == iterator_end(it), "expected end");
}

TEST(should_handle_numbers) {
  char *json = "12345";
  struct array_list_token *tokens = tokens_from_json_string(json);
  struct iterator *it = iterator_begin((struct array_list *)tokens);
  cassert(iterator_element_token(it)->type == number, "expected number");
  cassert(token_get_number(iterator_element_token(it)) == 12345, "expected 12345");
}

TEST(should_handle_booleans_and_null) {
  char *json = "true false null";
  struct array_list_token *tokens = tokens_from_json_string(json);
  struct iterator *it = iterator_begin((struct array_list *)tokens);
  
  cassert(iterator_element_token(it)->type == boolean, "expected true");
  cassert(token_get_bool(iterator_element_token(it)) == 1, "expected 1");
  iterator_increment(it);
  
  cassert(iterator_element_token(it)->type == space, "expected space");
  iterator_increment(it);

  cassert(iterator_element_token(it)->type == boolean, "expected false");
  cassert(token_get_bool(iterator_element_token(it)) == 0, "expected 0");
  iterator_increment(it);

  cassert(iterator_element_token(it)->type == space, "expected space");
  iterator_increment(it);

  cassert(iterator_element_token(it)->type == nullish, "expected null");
}
