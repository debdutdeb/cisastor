#pragma once

#include "containers/array_list.h"
enum token_type {
  left_brace,
  quote,
  colon,
  words,
  number,
  boolean,
  nullish,
  comma,
  right_brace,
  newline,
  space,
  left_square_bracket,
  right_square_bracket,
  eof,
};

typedef struct token {
  enum token_type type;
  union {
    char *value;
    int num;
  };
} token;

array_list_init(token);

int token_get_number(struct token *tok);
char *token_get_words(struct token *tok);
int token_get_bool(struct token *tok);

struct array_list_token *tokens_from_json_string(char *json);

char *token_to_string(struct token* tok);
