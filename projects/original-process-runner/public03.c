#include <assert.h>
#include <stdio.h>

#include "memory-checking.h"
#include "sss.h"

/*
 * Exercises multiple configured compilation commands, then verifies that
 * command storage is released without heap inconsistencies or leaks.
 */
int main(void) {
  Commands commands;

  setup_memory_checking();

  commands = read_commands("compile-commands02+03", "test-commands02+03");
  assert(compile_program(commands) == 1);

  clear_commands(&commands);
  check_heap();

  printf("Win!\n");
  return 0;
}
