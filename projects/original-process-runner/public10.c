#include <stdio.h>
#include "sss.h"

/*
 * Verifies that read_commands() terminates when its source file cannot be
 * opened. Only the line printed before the call should reach standard output.
 */
int main(void) {
  Commands commands;

  printf("Win!\n");

  commands = read_commands("a-file-that-does-not-exist", "test-commands10");
  compile_program(commands);

  return 0;
}
