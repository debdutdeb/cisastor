#include "functions.h"

// Returns 1 if match is a substring of src, 0 otherwise.
uint8_t strcmp_at_any(const char *const src, const char *const match) {
  if (*match == '\0') {
    return 1;
  }

  for (int i = 0; src[i] != '\0'; i++) {
    int j = 0;
    while (match[j] != '\0' && src[i + j] == match[j]) {
      j++;
    }
    if (match[j] == '\0') {
      return 1;
    }
  }

  return 0;
}
