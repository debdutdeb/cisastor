#include "string.h"
#include "iterator.h"
#include "arena.h"

#include <string.h>

#define to_array_list_char(str) cast(struct array_list_char *, str)
#define to_array_list(str) cast(struct array_list *, str)

string *string_create(const char *const c) {
  size_t n = strlen(c);
  struct array_list_char *al = array_list_create_char_with_capacity(n);
  const char *buffer = c;
  while (*buffer != 0)
    array_list_append_char(al, *(buffer++));
  return al;
}

string *string_from_array_list(struct array_list_char *al) { return al; }

string *string_concat(const string *const str1, const string *const str2) {
  size_t n1 = string_length(str1);
  size_t n2 = string_length(str2);
  struct array_list_char *res = array_list_create_char_with_capacity(n1 + n2);
  
  for (size_t i = 0; i < n1; i++) {
    string_append_char(res, string_char_at(str1, i));
  }
  for (size_t i = 0; i < n2; i++) {
    string_append_char(res, string_char_at(str2, i));
  }
  return res;
}

string *string_substring(const string *const str, int idx1, int idx2) {
  string *substring = string_create("");
  size_t len = string_length(str);
  if (idx1 < 0) idx1 = 0;
  if (idx2 > (int)len) idx2 = (int)len;
  
  for (int i = idx1; i < idx2; i++) {
    string_append_char(substring, string_char_at(str, i));
  }
  return substring;
}

int string_cmp(const string *const str1, const string *const str2) {
  size_t n1 = string_length(str1);
  size_t n2 = string_length(str2);
  size_t min_n = n1 < n2 ? n1 : n2;
  
  int res = memcmp(to_array_list(str1)->region, to_array_list(str2)->region, min_n);
  if (res == 0) {
    if (n1 < n2) return -1;
    if (n1 > n2) return 1;
    return 0;
  }
  return res;
}

char *string_to_primitive(string *str) {
  size_t n = string_length(str);
  char *c = string_create_empty(n);
  memcpy(c, to_array_list(str)->region, n);
  return c;
}
