#include <ctype.h>
#include <stdio.h>

#include "arena.h"
#include "json.h"
#include "macros.h"
#include "string_view.h"

const char *string_view_get_json_key(struct string_view *sv) {
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
    break;
  case '{':
    value->kind = json_object;
    break;
  case '"':
    value->kind = json_string;
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

  while (1) {
    const char *key = string_view_get_json_key(sv);
    if (null == key) {
      return null;
    }

    string_view_consume_till_alnum(sv);
    char sep = string_view_next_char(sv);
    if (':' != sep) {
      fprintf(stderr, "json expected to seperate with ':' but received '%c'\n",
              sep);
      return null;
    }
    string_view_consume_till_alnum(sv);
  }

  return null;
}
