#include "testing.h"

#include "shell.h"

TEST(should_run_basic_command) {
  if (run_command("sh -c \"ls > /dev/null\"") == 1) {
    fail("command \"ls\" should have run successfully");
  }
}
