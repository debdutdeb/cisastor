#include <string.h>

enum json_kind {
  json_null,
  json_number,
  json_bool,
  json_string,
  json_list,
  json_object,
};

struct json_value {
  enum json_kind kind;
  union {
    int boolean;
    double number;
    char *string;
    struct {
      struct json_value **items;
      size_t length;
    } list;
    struct {
      struct json_value *fields;
      size_t length;
    } object;
  };
};

struct json_field {
  char *key;
  struct json_value *value;
};

struct json_view {
  const char *json;
};
