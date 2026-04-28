#include "token.h"

#include "containers/array_list.h"

typedef struct token json_lexer_token;

array_list_init(json_lexer_token);

struct array_list* tokens_from_json_string(char *json) {
  struct array_list * token_list = array_list_create_json_lexer_token();
  if (null == token_list) {
    return null;
  }


  return token_list;
}
