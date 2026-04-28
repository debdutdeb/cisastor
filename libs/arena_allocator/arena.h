#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"

byte *arena_alloc(size_t size);
char *string_create_empty(size_t characters);

#define aalloc arena_alloc
#define reaalloc(a, b) aalloc(b)
