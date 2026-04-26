#include <ctype.h>

#include "string_view.h"
#include "arena.h"
#include "macros.h"

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

void string_view_consume_till_alnum(struct string_view* sv) {
	string_view_consume_chars(sv, " \t\n");
}
