#include <assert.h>
#include <stdio.h>

#include "sss.h"

/*
 * Executes command sets that exercise input redirection.
 */
int main(void) {
  Commands commands = read_commands("compile-commands09", "test-commands09");

  assert(compile_program(commands) == 1);
  assert(test_program(commands) == 3);

  printf("Win!\n");

  return 0;
}
