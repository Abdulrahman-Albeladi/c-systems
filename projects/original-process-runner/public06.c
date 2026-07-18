#include <assert.h>
#include <stdio.h>

#include "memory-checking.h"
#include "sss.h"

/*
 * Exercises command parsing, compilation, test execution, and command
 * cleanup under the project's memory-checking instrumentation.
 */
int main(void) {
  Commands commands;

  setup_memory_checking();

  commands = read_commands("compile-commands05+06", "test-commands05+06");

  assert(compile_program(commands) == 1);
  assert(test_program(commands) == 3);

  clear_commands(&commands);
  check_heap();

  printf("Win!\n");

  return 0;
}
