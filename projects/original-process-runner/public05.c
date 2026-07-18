#include <assert.h>
#include <stdio.h>

#include "sss.h"

int main(void) {
  Commands commands = read_commands("compile-commands05+06",
                                   "test-commands05+06");

  assert(compile_program(commands) == 1);
  assert(test_program(commands) == 3);

  printf("Win!\n");
  return 0;
}
