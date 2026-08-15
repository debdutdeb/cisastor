/* vim: set tabstop=2 shiftwidth=2 expandtab: */
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"
#include "containers/string_view.h"
#include "arena.h"

enum token_kind {
  keyword,
  identifier,
  star,
  left_brace,
  right_brace,
  semicolon,
  cisastor_comment,
};

struct token {
  enum token_kind kind;
  void *data;
};

char *token_kind_to_string(enum token_kind kind) {
  switch (kind) {
  case keyword:
    return "KEYWORD";
  case identifier:
    return "IDENTIFIER";
  case star:
    return "STAR";
  case left_brace:
    return "LEFT_BRACE";
  case right_brace:
    return "RIGHT_BRACE";
  case semicolon:
    return "SEMICOLON";
  case cisastor_comment:
    return "GENERATOR_START";
  default:
    return null;
  }
}

void _print_token(struct token *tok) {
  printf("[%s(%s)]", token_kind_to_string(tok->kind), cast(char *, tok->data));
}

struct token_node {
  struct token *token;
  struct token_node *next;
};

struct token_node *token_node_create(enum token_kind kind, void *data) {
  struct token_node *node = arena_alloc(sizeof(struct token_node));
  if (null == node) {
    return null;
  }

  node->token = arena_alloc(sizeof(struct token));
  if (null == node->token) {
    return null;
  }
  node->token->kind = kind;
  node->token->data = data;
  return node;
}

enum tokenize_state {
  state_expecting_magic_comment,
  state_expecting_struct,
  state_inside_struct,
};

void tokenizing_state_advance(enum tokenize_state *state) {
  switch (*state) {
  case state_expecting_magic_comment:
    *state = state_expecting_struct;
    break;
  case state_expecting_struct:
    *state = state_inside_struct;
    break;
  case state_inside_struct:
    *state = state_expecting_magic_comment;
    break;
  }
}

void emit_node_or_fail(struct token_node **root, enum token_kind kind,
                       void *data) {
  struct token_node *node = token_node_create(kind, data);
  if (null == node) {
    perror("token_allocation_failed");
    exit(1);
  }
  (*root)->next = node;
  *root = node;
}

struct token_node *tokenize(char *stream) {
  enum tokenize_state state = state_expecting_magic_comment;

  // FIXME: waste of a node
  struct token_node *root = token_node_create(cisastor_comment, "");
  if (null == root) {
    return null;
  }

  struct token_node *current_node = root;

  struct string_view *sv = string_view_create(stream);

  int curr = 0;
  while ((curr = string_view_next_char(sv)) != '\0') {
    string_view_consume_chars(
        sv, " \t"); // skip until an alphanumeric char appears;
    switch (state) {
    case state_expecting_magic_comment:
      if (curr != '/') {
        continue;
      }
      int pos = string_view_current_pos(sv);
      if (pos == 0 || string_view_char_at(sv, pos - 1) == 10) {
        // newline or first line, comment is valid
        int next = string_view_next_char(sv);
        if (next == '/') {
          const char *const look_for = "cisastor::json generate";

          int matched = string_view_compare_word(sv, look_for);

          if (matched == 0) {
            // read the rest
            emit_node_or_fail(&current_node, cisastor_comment, "generate");
            tokenizing_state_advance(&state);
          }

          string_consume_till_end_of_line(sv);
          continue;
        }
      }
      break;
    case state_expecting_struct:
      if (curr != 's') {
        continue;
      }
      if (string_view_compare_word(sv, "truct") != 0) {
        fprintf(stderr, "tokenizer error: expected keyword 'struct' got '%s'\n",
                string_view_consume_word(sv));
        return null;
      }
      emit_node_or_fail(&current_node, keyword, "struct");

      string_view_consume_chars(sv, " \t\n");

      const char *const struct_name = string_view_consume_word(sv);
      if (null == struct_name) {
        return null;
      }

      emit_node_or_fail(&current_node, identifier, cast(void *, struct_name));

      string_view_consume_chars(sv, " \t\n");

      int expected_left_brace = string_view_next_char(sv);
      if ('\0' == expected_left_brace) {
        fprintf(stderr,
                "tokenizer error: expected left opening brace, got EOF\n");
        return null;
      } else if ('{' != expected_left_brace) {
        fprintf(stderr,
                "tokenizer error: expected left opening brace, got '%c'\n",
                expected_left_brace);
        return null;
      }

      emit_node_or_fail(&current_node, left_brace, "{");
      string_view_consume_chars(sv, " \t\n");
      tokenizing_state_advance(&state);
      break;
    case state_inside_struct:
      // until an alphanumeric appears
      string_view_consume_chars(sv, " \t\n");

      if (curr == '}') {
        emit_node_or_fail(&current_node, right_brace, "}");
        cast(void, string_view_next_char(sv)); // consume before ending;
        tokenizing_state_advance(&state);
        break;
      }
      // w1   w2   w3
      // enum type iden;
      // type iden;
      const char *word = string_view_consume_word(sv);
      string_view_consume_chars(sv, " \t\n");
      {
        char *word_1 = string_create_empty(strlen(word) + 1);
        word_1[0] = curr;
        strncpy(word_1 + 1, word, strlen(word));
        emit_node_or_fail(&current_node, keyword, cast(void *, word_1));
      }
      const char *word2 = string_view_consume_word(sv);
      string_view_consume_chars(sv, " \t\n");

      char *identifier_str = null;

      if (string_view_peek_char(sv) == ';') {
        // end of line;
        identifier_str = word2;
      } else {
        emit_node_or_fail(&current_node, keyword, cast(void *, word2));
        const char *word3 = string_view_consume_word(sv);
        string_view_consume_chars(sv, " \t\n");
        identifier_str = word3;
      }
      string_view_consume_chars(sv, " \t\n");
      if (string_view_peek_char(sv) == '*') {
        cast(void, string_view_next_char(sv));
        emit_node_or_fail(&current_node, star, "*");
      }
      string_view_consume_chars(sv, " \t\n");
      emit_node_or_fail(&current_node, identifier, identifier_str);
      int c = 0;
      if ((c = string_view_next_char(sv)) != ';') {
        fprintf(stderr,
                "tokenizer error: expected statement terminator ';', got "
                "'%c'\n",
                c);
        return null;
      }
      emit_node_or_fail(&current_node, semicolon, ";");

      break;
    }
  }

  return root;
}

void tokenize_and_print(char *stream) {
  struct token_node *root = tokenize(stream);
  assert(root != null && "root should not be empty");
  struct token_node *node =
      root->next; // skip the first one, wasted resource yes;
  while (null != node) {
    _print_token(node->token);
    node = node->next;
  }
}

int main() {
  tokenize_and_print("//cisastor::json generate\nstruct user {int num;}");
  putchar(10);
  tokenize_and_print(
      "//cisastor::json generate\n\n      struct address\n{\nint num;}");
  putchar(10);
  tokenize_and_print(
      "//cisastor::json generate\n\n      struct address\n{\nchar * road;}");
  return 0;
}
