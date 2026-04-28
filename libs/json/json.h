#include <string.h>

enum json_kind {
  json_null,
  json_number,
  json_bool,
  json_string,
  json_list,
  json_object,
};

struct json_value_list_item {
	struct json_value* item;
	struct json_value_list_item *next;
};

struct json_value {
  enum json_kind kind;
  union {
    int boolean;
    double number;
    char *string;
	struct json_value_list_item items;
    struct {
      struct json_value *fields;
    } object;
  };
};

struct json_field {
  char *key;
  struct json_value *value;
};

