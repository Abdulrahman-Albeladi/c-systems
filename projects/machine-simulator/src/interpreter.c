#include "machine.h"
#include "interpreter.h"

#include <stdio.h>

static int decode_opcode(Word instruction) {
  return (int)((instruction >> 28) & 0xFu);
}

static int decode_reg1(Word instruction) {
  return (int)((instruction >> 24) & 0xFu);
}

static int decode_reg2(Word instruction) {
  return (int)((instruction >> 20) & 0xFu);
}

static int decode_reg3(Word instruction) {
  return (int)((instruction >> 16) & 0xFu);
}

static unsigned int decode_imm16(Word instruction) {
  return (unsigned int)(instruction & 0xFFFFu);
}

static int is_aligned_word_address(unsigned int address) {
  return (address % 4u) == 0u;
}

static int is_memory_address_in_range(unsigned int address) {
  return is_aligned_word_address(address) && (address / 4u) < MAX_MEMORY;
}

int load_program(Machine *const machine, const Word program[], int program_size) {
  int i;

  if (machine == NULL || program == NULL || program_size < 0 || program_size > MAX_MEMORY) {
    return 0;
  }

  for (i = 0; i < program_size; ++i) {
    machine->memory[i] = program[i];
  }

  machine->registers[NUM_REGISTERS - 1] = 0;
  return 1;
}

Status run_SPIM_program(Machine *const machine, int max_instr,
                        int *const num_instr, int trace_flag) {
  (void)trace_flag;

  while (1) {
    unsigned int pc;
    Word instruction;
    int opcode;
    int reg1;
    int reg2;
    int reg3;
    unsigned int imm16;

    if (machine == NULL || num_instr == NULL) {
      return PARAMETER_ERROR;
    }

    if (max_instr <= 0 || *num_instr >= max_instr) {
      return TIMEOUT;
    }

    pc = (unsigned int)machine->registers[NUM_REGISTERS - 1];
    if (!is_memory_address_in_range(pc)) {
      return INVALID_INSTRUCTION;
    }

    instruction = machine->memory[pc / 4u];
    if (!valid_instruction(instruction)) {
      return INVALID_INSTRUCTION;
    }

    opcode = decode_opcode(instruction);
    reg1 = decode_reg1(instruction);
    reg2 = decode_reg2(instruction);
    reg3 = decode_reg3(instruction);
    imm16 = decode_imm16(instruction);

    switch (opcode) {
      case 0:
        return HALTED;

      case 1:
        machine->registers[reg1] = machine->registers[reg2] + machine->registers[reg3];
        machine->registers[NUM_REGISTERS - 1] += 4;
        break;

      case 2:
        machine->registers[reg1] = machine->registers[reg2] * machine->registers[reg3];
        machine->registers[NUM_REGISTERS - 1] += 4;
        break;

      case 3:
        machine->registers[reg1] = -machine->registers[reg2];
        machine->registers[NUM_REGISTERS - 1] += 4;
        break;

      case 4:
        machine->registers[reg1] = machine->registers[reg2] << imm16;
        machine->registers[NUM_REGISTERS - 1] += 4;
        break;

      case 5:
        machine->registers[reg1] = machine->registers[reg2] && machine->registers[reg3];
        machine->registers[NUM_REGISTERS - 1] += 4;
        break;

      case 6:
        machine->registers[reg1] = !machine->registers[reg2];
        machine->registers[NUM_REGISTERS - 1] += 4;
        break;

      case 7:
        if (!is_memory_address_in_range(imm16)) {
          return INVALID_INSTRUCTION;
        }
        machine->registers[reg1] = machine->memory[imm16 / 4u];
        machine->registers[NUM_REGISTERS - 1] += 4;
        break;

      case 8:
        machine->registers[reg1] = (int)imm16;
        machine->registers[NUM_REGISTERS - 1] += 4;
        break;

      case 9:
        if (!is_memory_address_in_range(imm16)) {
          return INVALID_INSTRUCTION;
        }
        machine->memory[imm16 / 4u] = (Word)machine->registers[reg1];
        machine->registers[NUM_REGISTERS - 1] += 4;
        break;

      case 10:
        machine->registers[reg1] = machine->registers[reg2];
        machine->registers[NUM_REGISTERS - 1] += 4;
        break;

      case 11:
        if (machine->registers[reg1] != machine->registers[reg2]) {
          if (!is_memory_address_in_range(imm16)) {
            return INVALID_INSTRUCTION;
          }
          machine->registers[NUM_REGISTERS - 1] = (int)imm16;
        } else {
          machine->registers[NUM_REGISTERS - 1] += 4;
        }
        break;

      case 12:
        if (scanf("%d", &machine->registers[reg1]) != 1) {
          return INVALID_INSTRUCTION;
        }
        machine->registers[NUM_REGISTERS - 1] += 4;
        break;

      case 13:
        printf("%d\n", machine->registers[reg1]);
        machine->registers[NUM_REGISTERS - 1] += 4;
        break;

      default:
        return INVALID_INSTRUCTION;
    }

    ++(*num_instr);
  }
}

int reset(Machine *const machine) {
  if (machine == NULL) {
    return 0;
  }

  machine->registers[NUM_REGISTERS - 1] = 0;
  return 1;
}
