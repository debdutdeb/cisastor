#pragma once

#include <stdint.h>
#include <string.h>

struct string_view {
  char *stream;
  char *ptr;

  size_t length;
};

struct string_view *string_view_create(char *stream);
char string_view_peek_char(struct string_view *sv);
char string_view_next_char(struct string_view *sv);
int string_view_current_pos(struct string_view *sv);
char string_view_char_at(struct string_view *sv, int pos);
int string_view_compare_word(struct string_view *sv, const char *const word);
void string_view_consume_chars(struct string_view *sv,
                               char *null_terminated_char_list);
void string_consume_till_end_of_line(struct string_view *sv);
void string_view_consume_whitespace(struct string_view *sv);
const char *const string_view_consume_word(struct string_view *sv);
void string_view_consume_till_alnum(struct string_view *sv);
char *string_view_consume_word_until_chars(struct string_view *sv, char *chars);
