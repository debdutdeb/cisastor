#include "token.h"

#include "arena.h"
#include "containers/array_list.h"
#include "containers/string_view.h"
#include "macros.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct token json_lexer_token;

array_list_init(json_lexer_token);

char *token_type_to_string(enum token_type type) {
  switch (type) {
  case left_brace:
    return "LEFT_BRACE";
  case quote:
    return "QUOTE";
  case colon:
    return "COLON";
  case words:
    return "WORDS";
  case number:
    return "NUMBER";
  case boolean:
    return "BOOLEAN";
  case nullish:
    return "NULL";
  case comma:
    return "COMMA";
  case right_brace:
    return "RIGHT_BRACE";
  case newline:
    return "NEWLINE";
  case space:
    return "SPACE";
  default:
    return "UNKNOWN";
  }
}

char *token_to_string(struct token *tok) {
  char *tok_str = token_type_to_string(tok->type);
  const char *const extra_chars = "[]()";
  size_t length = strlen(tok_str) + strlen(extra_chars);
  switch (tok->type) {
  case words:
    if (null == tok->value) {
      tok->value = "unknown";
    }
    length += strlen(tok->value);
    char *word = string_create_empty(length);
    if (null == word) {
      return null;
    }
    sprintf(word, "[%s(%s)]", tok_str, tok->value);
    return word;
  case number: {
    int digits = 1;
    int num = tok->num;
    while (num /= 10)
      digits++;
    char *word = string_create_empty(length + digits);
    if (null == word) {
      return null;
    }
    sprintf(word, "[%s(%d)]", tok_str, tok->num);
    return word;
  }
  case boolean: {
    char *value = tok->num ? "true" : "false";
    char *word = string_create_empty(length + strlen(value));
    if (null == word) {
      return null;
    }
    sprintf(word, "[%s(%s)]", tok_str, value);
    return word;
  }
  default: {
    char *word = string_create_empty(strlen(tok_str) + 2);
    if (null == word) {
      return null;
    }
    sprintf(word, "[%s]", tok_str);
    return word;
  }
  }
}

void append_token_to_token_list_or_fail(struct array_list *token_list,
                                        enum token_type type, char *value,
                                        int num) {
  struct token tok = {.type = type};
  if (type == words) {
    tok.value = value;
  } else if (type == number || type == boolean) {
    tok.num = num;
  }
  if (null == array_list_append_json_lexer_token(token_list, tok)) {
    fprintf(stderr, "failed to append new token with id %s, %s",
            token_type_to_string(type), cast(char *, value));
    exit(1);
  }
}

int isnumber(int c) { return '0' <= c && '9' >= c; }

struct array_list *tokens_from_json_string(char *json) {
  struct array_list *token_list = array_list_create_json_lexer_token();
  if (null == token_list) {
    return null;
  }

  const struct string_view *sv = string_view_create(json);

  int curr;
  while ((curr = string_view_next_char(sv)) != '\0') {
    switch (curr) {
    case '{':
      append_token_to_token_list_or_fail(token_list, left_brace, null, 0);
      break;
    case '}':
      append_token_to_token_list_or_fail(token_list, right_brace, null, 0);
      break;

    case '"':
      append_token_to_token_list_or_fail(token_list, quote, null, 0);
      break;
    case ':':
      append_token_to_token_list_or_fail(token_list, colon, null, 0);
      break;
    case 't':
      if (0 == string_view_compare_word(sv, "rue")) {
        append_token_to_token_list_or_fail(token_list, boolean, null, 1);
        break;
      }
    case 'f':
      if (0 == string_view_compare_word(sv, "alse")) {
        append_token_to_token_list_or_fail(token_list, boolean, null, 0);
        break;
      }
    case 'n':
      if (0 == string_view_compare_word(sv, "ull")) {
        append_token_to_token_list_or_fail(token_list, nullish, null, 0);
        break;
      }
    case ',':
      append_token_to_token_list_or_fail(token_list, comma, null, 0);
      break;
    case '\n':
      append_token_to_token_list_or_fail(token_list, newline, null, 0);
      break;
    case ' ':
    case '\t':
      append_token_to_token_list_or_fail(token_list, space, null, 0);
      break;
    default:
      if (isnumber(curr)) {
        int num = curr - '0';
        int c;
        while ((c = string_view_peek_char(sv)) != '\0' && isnumber(c)) {
          cast(void, string_view_next_char(sv));
          num *= 10;
          num += (c - '0');
        }
        append_token_to_token_list_or_fail(token_list, number, null, num);
        break;
      } else {
        const char *word = string_view_consume_word(sv);
        char *word2 = string_create_empty(strlen(word) + 1);
        word2[0] = curr;
        strncpy(word2 + 1, word, strlen(word));
        append_token_to_token_list_or_fail(token_list, words, word2, 0);
        break;
      }
    }
  }

  return token_list;
}
