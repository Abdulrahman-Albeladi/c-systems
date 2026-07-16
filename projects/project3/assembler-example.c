#include <stdio.h>

#include "assembler.h"
#include "interpreter.h"
#include "machine.h"

int main(void) {
  Machine spim = {{0}};
  Word memory_arr[MAX_MEMORY] = {0};
  int num_instructions_read;
  int num_instructions_executed;

  num_instructions_read = assemble("assembly-program", memory_arr);

  if (num_instructions_read == 0) {
    printf("Couldn't assemble program, or no instructions were read.\n");
    return 0;
  }

  printf("The program read was:\n");
  disassemble(memory_arr, 0, num_instructions_read);

  printf("\nNow loading and running the program:\n");
  load_program(&spim, memory_arr, num_instructions_read);
  run_SPIM_program(&spim, 100, &num_instructions_executed, 0);

  return 0;
}
