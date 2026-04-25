/* vim: set tabstop=2 shiftwidth=2 expandtab: */
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"

#ifndef ARENA_SIZ
#define ARENA_SIZ 1024
#endif

struct memory_arena {
  char *chunk;

  char *ptr;

  int current_size;
};

struct memory_arena arena = {};

void arena_free() { free(arena.chunk); }

void *arena_alloc(size_t size) {
  if (null == arena.chunk) {
    // sizeof(char) == 1 byte most of the time;
    arena.chunk = malloc(sizeof(char) * ARENA_SIZ);
    if (null == arena.chunk) {
      perror("arena_alloc");
      return null;
    }

    arena.current_size = ARENA_SIZ;
    arena.ptr = arena.chunk;

    atexit(cast(void (*)(void), arena_free));
  }

  void *ptr = arena.ptr;
  void *chunk = arena.chunk;

  void *end = chunk + arena.current_size;

  if (end - ptr < size) {
    size_t allocated_region = ptr - chunk;
    arena.chunk =
        realloc(arena.chunk, arena.current_size + (ARENA_SIZ * sizeof(char)));
    if (null == arena.chunk) {
      perror("arena_realloc");
      return null;
    }

    arena.current_size += ARENA_SIZ;
    end = arena.chunk + arena.current_size;
    arena.ptr = arena.chunk + allocated_region;
  }

  void *allocated = arena.ptr;

  arena.ptr += size;

  return allocated;
}

char *string_create_empty(size_t characters) {
  int length = characters + 1;
  char *str = arena_alloc(length * sizeof(char));
  memset(str, 0, length);
  return str;
}

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

struct string_view {
  char *stream;
  char *ptr;

  size_t length;
};

struct string_view *string_view_create(char *stream) {
  struct string_view *sview = arena_alloc(sizeof(struct string_view));
  if (null == sview) {
    return null;
  }

  sview->stream = stream;
  sview->ptr = stream;
  sview->length = strlen(stream);
  return sview;
}

char string_view_peek_char(struct string_view *sv) { return *sv->ptr; }

char string_view_next_char(struct string_view *sv) {
  if (string_view_peek_char(sv) == '\0') {
    return '\0';
  }

  return *sv->ptr++;
}

int string_view_current_pos(struct string_view *sv) {
  return sv->ptr - sv->stream - 1;
}

char string_view_char_at(struct string_view *sv, int pos) {
  if (pos >= sv->length) {
    return 0;
  }

  return sv->stream[pos];
}

int string_view_compare_word(struct string_view *sv, const char *const word) {
  size_t word_length = strlen(word);
  if ((sv->stream + sv->length - sv->ptr) < word_length) {
    return 0;
  }

  if (strncmp(sv->ptr, word, word_length) == 0) {
    sv->ptr += word_length;
    return 0;
  }

  return 1;
}

void string_view_consume_chars(struct string_view *sv,
                               char *null_terminated_char_list) {
  int c, matched;
  while ((c = string_view_peek_char(sv)) != '\0') {
    matched = 0;
    for (int i = 0, j = 0; (j = null_terminated_char_list[i]) != '\0'; i++) {
      if (c == j) {
        matched = 1;
        break;
      }
    }
    if (!matched) {
      return;
    }
    cast(void, string_view_next_char(sv));
  }
}

void string_consume_till_end_of_line(struct string_view *sv) {
  int c;
  while ((c = string_view_peek_char(sv)) != '\n' && c != '\0') {
    cast(void, string_view_next_char(sv));
  }
}

void string_view_consume_whitespace(struct string_view *sv) {
  string_view_consume_chars(sv, " \t");
}

const char *const string_view_consume_word(struct string_view *sv) {
  char *start = sv->ptr;
  int c;
  while (isalnum((c = string_view_peek_char(sv))) || c == '_') {
    cast(void, string_view_next_char(sv));
  }
  char *end = sv->ptr;
  size_t length = end - start;
  char *word = string_create_empty(length);
  if (null == word) {
    return null;
  }
  strncpy(word, start, length);
  return word;
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
            struct token_node *node =
                token_node_create(cisastor_comment, "generate");
            if (null == node) {
              return null;
            }

            current_node->next = node;
            current_node = node;
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
      struct token_node *node = token_node_create(keyword, "struct");
      if (null == node) {
        return null;
      }

      current_node->next = node;
      current_node = node;

      string_view_consume_chars(sv, " \t\n");

      const char *const struct_name = string_view_consume_word(sv);
      if (null == struct_name) {
        return null;
      }

      {
        struct token_node *node =
            token_node_create(identifier, cast(void *, struct_name));
        if (null == node) {
          return null;
        }

        current_node->next = node;
        current_node = node;
      }

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

      {
        struct token_node *node = token_node_create(left_brace, "{");
        if (null == node) {
          return null;
        }

        current_node->next = node;
        current_node = node;
      }

      tokenizing_state_advance(&state);
      break;
    case state_inside_struct:
      // until an alphanumeric appears
      string_view_consume_chars(sv, " \t\n");

      if (curr == '}') {
        {
          struct token_node *node = token_node_create(right_brace, "}");
          if (null == node) {
            return null;
          }
          current_node->next = node;
          current_node = node;
        }
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
        struct token_node *node =
            token_node_create(keyword, cast(void *, word));
        current_node->next = node;
        current_node = node;
      }
      const char *word2 = string_view_consume_word(sv);
      string_view_consume_chars(sv, " \t\n");

      if (string_view_peek_char(sv) == ';') {
        // end of line;
        {
          struct token_node *node =
              token_node_create(identifier, cast(void *, word2));
          current_node->next = node;
          current_node = node;
        }
      } else {
        {
          struct token_node *node =
              token_node_create(keyword, cast(void *, word2));
          current_node->next = node;
          current_node = node;
        }
        const char *word3 = string_view_consume_word(sv);
        string_view_consume_chars(sv, " \t\n");
        int c = 0;
        if ((c = string_view_next_char(sv)) != ';') {
          fprintf(stderr,
                  "tokenizer error: expected statement terminator ';', got "
                  "'%c'\n",
                  c);
          return null;
        }
        {
          struct token_node *node =
              token_node_create(identifier, cast(void *, word2));
          current_node->next = node;
          current_node = node;
        }
      }
      string_view_consume_chars(sv, " \t\n");

      break;
    }
  }

  return root;
}

int main() {
  struct token_node *root =
      tokenize("//cisastor::json generate\nstruct user {int num;}");
  assert(root != null && "root should not be empty");
  struct token_node *node =
      root->next; // skip the first one, wasted resource yes;
  while (null != node) {
    _print_token(node->token);
    node = node->next;
  }
  return 0;
}
