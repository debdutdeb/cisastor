#include "containers/array_list.h"
#include "containers/string.h"

#include <stdint.h>

enum json_kind {
  json_null,
  json_number,
  json_bool,
  json_string,
  json_list,
  json_object,
};

struct json_list {
  struct array_list *items;
};

struct json_object {
  struct array_list *fields;
};

struct json_value {
  enum json_kind kind;
  union {
    uint8_t boolean;
    double number;
    string *string;
    struct json_list *list;
    struct json_object *object;
  };
};

struct json_field {
  char *key;
  struct json_value *value;
};
