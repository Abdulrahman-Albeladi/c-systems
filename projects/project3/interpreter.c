#include "machine.h"
#include "interpreter.h"

#include <stdio.h>
#include <stdlib.h>

int load_program(Machine *const spim, const Word program[], int program_size) {
  int i;

  if (spim == NULL || program == NULL || program_size > MAX_MEMORY)
    return 0;

  if (program_size <= 0)
    return 1;

  for (i = 0; i < program_size; i++)
    spim->memory[i] = program[i];

  spim->registers[NUM_REGISTERS - 1] = 0;
  return 1;
}

Status run_SPIM_program(Machine *const spim, int max_instr,
                        int *const num_instr, int trace_flag) {
  (void)trace_flag;

  if (spim == NULL || num_instr == NULL)
    return PARAMETER_ERROR;

  while (max_instr > 0 && *num_instr < max_instr) {
    unsigned int nxt_addr;
    unsigned int nxt_instr;
    int opcode;
    int reg1;
    int reg2;
    int reg3;
    int mem_addr;

    nxt_addr = spim->registers[NUM_REGISTERS - 1];
    if (nxt_addr % 4 != 0 || nxt_addr / 4 >= MAX_MEMORY)
      return INVALID_INSTRUCTION;

    nxt_instr = spim->memory[nxt_addr / 4];
    if (!valid_instruction(nxt_instr))
      return INVALID_INSTRUCTION;

    opcode = (nxt_instr >> 28) & 0xf;
    reg1 = (nxt_instr >> 24) & 0xf;
    reg2 = (nxt_instr >> 20) & 0xf;
    reg3 = (nxt_instr >> 16) & 0xf;
    mem_addr = nxt_instr & 0xffff;

    switch (opcode) {
      case 0:
        return HALTED;
      case 1:
        spim->registers[reg1] = spim->registers[reg2] + spim->registers[reg3];
        break;
      case 2:
        spim->registers[reg1] = spim->registers[reg2] * spim->registers[reg3];
        break;
      case 3:
        spim->registers[reg1] = -spim->registers[reg2];
        break;
      case 4:
        spim->registers[reg1] = spim->registers[reg2] << mem_addr;
        break;
      case 5:
        spim->registers[reg1] = spim->registers[reg2] && spim->registers[reg3];
        break;
      case 6:
        spim->registers[reg1] = !spim->registers[reg2];
        break;
      case 7:
        if (mem_addr % 4 != 0 || mem_addr / 4 >= MAX_MEMORY)
          return INVALID_INSTRUCTION;
        spim->registers[reg1] = spim->memory[mem_addr / 4];
        break;
      case 8:
        spim->registers[reg1] = mem_addr;
        break;
      case 9:
        if (mem_addr % 4 != 0 || mem_addr / 4 >= MAX_MEMORY)
          return INVALID_INSTRUCTION;
        spim->memory[mem_addr / 4] = spim->registers[reg1];
        break;
      case 10:
        spim->registers[reg1] = spim->registers[reg2];
        break;
      case 11:
        break;
      case 12:
        scanf("%d", &spim->registers[reg1]);
        break;
      case 13:
        printf("%d\n", spim->registers[reg1]);
        break;
    }

    if (opcode == 11 && spim->registers[reg1] != spim->registers[reg2])
      spim->registers[NUM_REGISTERS - 1] = mem_addr;
    else
      spim->registers[NUM_REGISTERS - 1] += 4;

    (*num_instr)++;
  }

  return TIMEOUT;
}

int reset(Machine *const spim) {
  if (spim == NULL)
    return 0;

  spim->registers[NUM_REGISTERS - 1] = 0;
  return 1;
}
