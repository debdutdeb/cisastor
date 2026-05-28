#pragma once

#include "array_list.h"
#include "string.h"

typedef const char *const_char_ptr;
array_list_init(const_char_ptr);

// Helps build the final string from multiple parts
typedef struct array_list_const_char_ptr string_builder;

string_builder *string_builder_create();
string_builder *string_builder_join(string_builder *sb, const char *const cstr);

string *string_builder_build(string_builder *sb);
