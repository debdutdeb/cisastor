#include "token.h"

#include "containers/iterator.h"

#include <stdio.h>

typedef struct token token;

array_list_init(token);

int main() {
  {
    struct array_list *tokens =
        tokens_from_json_string("{ \"id\": 1, \"active\": true, \"scores\": "
                                "[10, 20], \"meta\": { \"verified\": null } }");

    for (struct iterator *it = iterator_begin(tokens);
         iterator_element(it) != iterator_end(it); iterator_increment(it)) {
      struct token *tok = iterator_element_token(it);
      printf("%s", token_to_string(tok));
    }
    putchar(10);
  }
}
