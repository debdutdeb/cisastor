#pragma once

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
};

struct token {
  enum token_type type;
  union {
    char *value;
    int num;
  };
};

int token_get_number(struct token *tok);
char *token_get_words(struct token *tok);
int token_get_bool(struct token *tok);

struct array_list *tokens_from_json_string(char *json);

char *token_to_string(struct token* tok);
