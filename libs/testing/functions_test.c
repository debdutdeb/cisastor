#include "functions.h"
#include "testing.h"

#include "functions.h"

#define CISASTOR_TESTING_HALT_ON_FAIL 0

TEST(should_match_full_string) {
  char *s1 = "abcd";
  char *s2 = "abcd";

  if (!strcmp_at_any(s1, s2)) {
    failf("expected \"%s\" to match with itself\n", s1);
  }
}

TEST(should_match_first_characters) {
  char *s1 = "abcd_efgh";
  char *s2 = "abcd";

  if (!strcmp_at_any(s1, s2)) {
    failf("expected \"%s\" to be caught for \"%s\" source string\n", s2, s1);
  }
}

TEST(should_match_middle_characters) {
  char *s1 = "abcd_efgh";
  char *s2 = "cd_e";

  if (!strcmp_at_any(s1, s2)) {
    failf("expected \"%s\" to be caught for \"%s\" source string\n", s2, s1);
  }
}

TEST(should_match_last_characters) {
  char *s1 = "abcd_efgh";
  char *s2 = "efgh";

  if (!strcmp_at_any(s1, s2)) {
    failf("expected \"%s\" to be caught for \"%s\" source string\n", s2, s1);
  }
}

TEST(should_not_match_nonexistent_substring) {
  char *s1 = "abcd_efgh";
  char *s2 = "ijkl";

  if (strcmp_at_any(s1, s2)) {
    failf("expected \"%s\" to not be caught for \"%s\" source string\n", s2, s1);
  }
}

TEST(should_not_match_partial_overlap) {
  char *s1 = "abcdef";
  char *s2 = "defg";

  if (strcmp_at_any(s1, s2)) {
    failf("expected \"%s\" to not partially match \"%s\"\n", s2, s1);
  }
}

TEST(should_match_single_character) {
  char *s1 = "abcdef";
  char *s2 = "d";

  if (!strcmp_at_any(s1, s2)) {
    failf("expected \"%s\" to be caught for \"%s\"\n", s2, s1);
  }
}

TEST(should_not_match_empty_source) {
  char *s1 = "";
  char *s2 = "abc";

  if (strcmp_at_any(s1, s2)) {
    failf("expected \"%s\" to not match empty source string\n", s2);
  }
}

TEST(should_match_empty_string) {
  char *s1 = "abcdef";
  char *s2 = "";

  if (!strcmp_at_any(s1, s2)) {
    failf("expected empty string to match \"%s\"\n", s1);
  }
}

TEST(should_match_repeated_patterns) {
  char *s1 = "abcabcabc";
  char *s2 = "cab";

  if (!strcmp_at_any(s1, s2)) {
    failf("expected \"%s\" to be caught for \"%s\"\n", s2, s1);
  }
}

TEST(should_match_specific_case_pattern) {
  char *s1 = "should_match_repeated_patterns";
  char *s2 = "patterns";
  if (!strcmp_at_any(s1, s2)) {
    failf("expected \"%s\" to be caught for \"%s\"\n", s2, s1);
  }
}

TEST(should_match_repeating_pattern_after_initial_mismatch) {
  char *s1 = "aaabaaaab";
  char *s2 = "aaaab";

  if (!strcmp_at_any(s1, s2)) {
    failf("expected \"%s\" to be caught for \"%s\"\n", s2, s1);
  }
}

TEST(should_match_repeating_prefix_later_in_string) {
  char *s1 = "abababc";
  char *s2 = "ababc";

  if (!strcmp_at_any(s1, s2)) {
    failf("expected \"%s\" to be caught for \"%s\"\n", s2, s1);
  }
}

TEST(should_match_after_multiple_partial_failures) {
  char *s1 = "aaaaaaab";
  char *s2 = "aaab";

  if (!strcmp_at_any(s1, s2)) {
    failf("expected \"%s\" to be caught for \"%s\"\n", s2, s1);
  }
}

TEST(should_match_repeating_character_suffix) {
  char *s1 = "bbbbbbbbbc";
  char *s2 = "bbbbbc";

  if (!strcmp_at_any(s1, s2)) {
    failf("expected \"%s\" to be caught for \"%s\"\n", s2, s1);
  }
}

TEST(should_not_match_repeating_pattern_with_wrong_ending) {
  char *s1 = "aaaaaaab";
  char *s2 = "aaaac";

  if (strcmp_at_any(s1, s2)) {
    failf("expected \"%s\" to not be caught for \"%s\"\n", s2, s1);
  }
}

TEST(should_match_overlapping_repeating_pattern) {
  char *s1 = "abababab";
  char *s2 = "ababab";

  if (!strcmp_at_any(s1, s2)) {
    failf("expected \"%s\" to be caught for \"%s\"\n", s2, s1);
  }
}

TEST(should_match_long_repeating_pattern_after_false_starts) {
  char *s1 = "abcabcabcd";
  char *s2 = "abcabcd";

  if (!strcmp_at_any(s1, s2)) {
    failf("expected \"%s\" to be caught for \"%s\"\n", s2, s1);
  }
}
