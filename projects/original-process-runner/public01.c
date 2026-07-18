#include <assert.h>
#include <stdio.h>

#include "sss.h"

int main(void) {
  Commands commands = read_commands("compile-commands01", "test-commands01");

  assert(compile_program(commands) == 1);
  printf("Win!\n");

  return 0;
}
