#include <assert.h>
#include <stdio.h>

#include "sss.h"

int main(void) {
  Commands commands = read_commands("compile-commands08", "test-commands08");

  assert(compile_program(commands) == 1);
  assert(test_program(commands) == 2);

  printf("Win!\n");
  return 0;
}
