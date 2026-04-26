#include <ctype.h>
#include <stdio.h>

#include "arena.h"
#include "json.h"
#include "macros.h"
#include "string_view.h"

void __log_unexpected_character(char expected, char got) {
  fprintf(stderr, "unexpected character '%c', expected '%c'\n", got, expected);
}

// Expects view to begin at the start of the line.
// Ends right after '"' is consumed.
const char *string_view_to_json_key(struct string_view *sv) {
  string_view_consume_till_alnum(sv);
  char start = string_view_next_char(sv);
  if ('"' != start) {
    fprintf(stderr, "expected key to start with '\"', got '%c'\n", start);
    return null;
  }
  const char *word = string_view_consume_word(sv);
  char end = string_view_next_char(sv);
  if ('"' != end) {
    fprintf(stderr, "expected key to end with '\"', got '%c'\n", start);
    return null;
  }
  return word;
}

// Expects view to start after ':', can include whitespaces.
// Ends once value token is consume, so ',' and whitespaces are left alone.
struct json_value *string_view_to_json_value(const struct string_view *sv) {
  struct json_value *value =
      cast(struct json_value *, aalloc(sizeof(struct json_value)));
  if (null == value) {
    perror("aalloc_json_value");
    return null;
  }
  string_view_consume_till_alnum(sv);
  char value_start = string_view_next_char(sv);
  switch (value_start) {
  case '[':
    value->kind = json_list;
    for (int i = 0; string_view_peek_char(sv) != ']'; i++) {
      struct json_value *item = string_view_to_json_value(sv);
      value->list.items[i] = item;
      value->list.length++;
    }
    break;
  case '{':
    value->kind = json_object;
    struct json_value *object_value = string_view_to_json_value(sv);
    break;
  case '"':
    value->kind = json_string;
    char *word = string_view_consume_word_until_chars(sv, "\"");
    value->string = word;
    int next_character = string_view_next_char(sv);
    if ('"' != next_character) {
      __log_unexpected_character('"', next_character);
      return null;
    }
    break;
  case 't':
  case 'f': {
    const char *word = string_view_consume_word(sv);
    if (strncmp(word, "rue", 3) == 0) {
      value->kind = json_bool;
      value->boolean = 1;
    } else if (strncmp(word, "alse", 4) == 0) {
      value->kind = json_bool;
      value->boolean = 0;
    } else {
      fprintf(stderr, "unknown keyword %c%s\n", value_start, word);
      return null;
    }
  }
  default:
    if (!isnumber(value_start) && value_start != '.') {
      fprintf(stderr, "unknown json value type %c\n", value_start);
      return null;
    }
    value->kind = json_number;
  }
}

struct json_view *parse(const char *const json) {
  if (null == json) {
    return null;
  }

  struct string_view *sv = string_view_create(json);
  string_view_consume_till_alnum(sv);

  char opening = string_view_next_char(sv);
  if ('{' != opening) {
    fprintf(stderr, "json expected to start with '{' but received '%c'\n",
            opening);
    return null;
  }

  string_view_consume_chars(sv, " \t\n");

  while (string_view_peek_char(sv) != '\0') {
    // beginning of the next line
    const char *key = string_view_to_json_key(sv);
    if (null == key) {
      return null;
    }
    string_view_consume_whitespace(sv);
    char sep = string_view_next_char(sv);
    if (':' != sep) {
      __log_unexpected_character(':', sep);
      return null;
    }
    string_view_consume_whitespace(sv);
    struct json_value *value = string_view_to_json_value(sv);
    if (null == value) {
      return null;
    }
    string_view_consume_whitespace(sv);
    // there should be a comma, or next alpha character is to be '}'
    int next = string_view_next_char(sv);
    if (',' == next) {
      string_view_consume_chars(sv, " \t\n");
      if ('}' == string_view_peek_char(sv)) {
        fprintf(stderr,
                "got '}', not expected as last field was terminated by ','\n");
        return null;
      }
      continue;
    }
    if ('\n' != next) {
      fprintf(stderr, "expected newline got '%c'\n", next);
      return null;
    }
    string_view_consume_whitespace(sv);
    next = string_view_next_char(sv);
    if ('}' != next) {
      __log_unexpected_character('}', next);
      return null;
    }
    break;
  }

  return null;
}
