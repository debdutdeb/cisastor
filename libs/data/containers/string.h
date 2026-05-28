#pragma once

#include "array_list.h"
#include "macros.h"
#include <stdint.h>

typedef struct array_list_char string;

// Returns the length of the string
#define string_length(str) array_list_get_length(cast(struct array_list *, str))
#define string_char_at(str, idx)                                               \
  *array_list_get_char_at(cast(struct array_list_char *, str), idx)
#define string_append_char(str, c)                                             \
  array_list_append_char(cast(struct array_list_char *, str), c)

string *string_concat(const string *const str1, const string *const str2);
string *string_substring(const string *const str, int idx1, int idx2);

// Same as strcmp
int string_cmp(const string *const str1, const string *const str2);

string *string_create(const char *const c);
string *string_from_array_list(struct array_list_char *al);

char *string_to_primitive(string *str);

char *const string_to_primitive_underlying(string *str);
