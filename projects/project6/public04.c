#include <assert.h>
#include <stdio.h>

#include "sss.h"

/*
 * Exercises a command set containing one compilation command and one
 * executable test command.
 */
int main(void) {
  Commands commands = read_commands("compile-commands04", "test-commands04");

  assert(compile_program(commands) == 1);
  assert(test_program(commands) == 1);

  printf("Win!\n");
  return 0;
}
