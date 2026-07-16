#include <assert.h>
#include <stdio.h>

#include "sss.h"

/*
 * Verifies that compile_program() reports failure when one or more configured
 * compilation commands exit unsuccessfully.
 */
int main(void) {
  Commands commands = read_commands("compile-commands07", "test-commands07");

  assert(compile_program(commands) == 0);

  printf("Win!\n");
  return 0;
}
