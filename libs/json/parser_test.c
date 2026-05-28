#include "testing.h"

#include "parser.h"
#include "token.h"

IT(should_fail) {
  struct array_list_token * al = array_list_create_token();
  struct token tok = { .type = left_brace };
  array_list_append_token_ptr(al, &tok);
  cassert(parse(al) == NULL, "currently it should fail");
}
