#include "machine.h"

#include <stdio.h>

/*
 * Independent reconstruction of a small fixed-width instruction encoder /
 * decoder. This file intentionally avoids assignment-specific scaffolding
 * while preserving the underlying concept: validate, encode, print, and
 * disassemble 32-bit instructions for a compact teaching ISA.
 */

enum {
  OPCODE_HALT = 0,
  OPCODE_ADD = 1,
  OPCODE_MUL = 2,
  OPCODE_NEG = 3,
  OPCODE_SHL = 4,
  OPCODE_AND = 5,
  OPCODE_NOT = 6,
  OPCODE_LW = 7,
  OPCODE_LI = 8,
  OPCODE_SW = 9,
  OPCODE_MOVE = 10,
  OPCODE_BNE = 11,
  OPCODE_READ = 12,
  OPCODE_WRITE = 13,
  OPCODE_MAX = 13,

  REGISTER_MIN = 0,
  REGISTER_MAX = 11,
  GENERAL_REGISTER_MIN = 1,
  GENERAL_REGISTER_MAX = 10,

  MEMORY_WORD_BYTES = 4,
  MEMORY_CAPACITY_WORDS = 12288,
  MEMORY_CAPACITY_BYTES = MEMORY_CAPACITY_WORDS * MEMORY_WORD_BYTES,

  ADDRESS_MASK_16 = 0xFFFFu,
  SHIFT_IMMEDIATE_MASK = 0xFFu,
  MAX_SHIFT_AMOUNT = 31u
};

static int opcode_operand_count(unsigned int opcode) {
  switch (opcode) {
    case OPCODE_HALT:
      return 0;
    case OPCODE_ADD:
    case OPCODE_MUL:
    case OPCODE_AND:
      return 3;
    case OPCODE_NEG:
    case OPCODE_SHL:
    case OPCODE_NOT:
    case OPCODE_MOVE:
    case OPCODE_BNE:
      return 2;
    case OPCODE_LW:
    case OPCODE_LI:
    case OPCODE_SW:
    case OPCODE_READ:
    case OPCODE_WRITE:
      return 1;
    default:
      return -1;
  }
}

static const char *opcode_name(unsigned int opcode) {
  switch (opcode) {
    case OPCODE_HALT:
      return "halt";
    case OPCODE_ADD:
      return "add";
    case OPCODE_MUL:
      return "mul";
    case OPCODE_NEG:
      return "neg";
    case OPCODE_SHL:
      return "shl";
    case OPCODE_AND:
      return "and";
    case OPCODE_NOT:
      return "not";
    case OPCODE_LW:
      return "lw";
    case OPCODE_LI:
      return "li";
    case OPCODE_SW:
      return "sw";
    case OPCODE_MOVE:
      return "move";
    case OPCODE_BNE:
      return "bne";
    case OPCODE_READ:
      return "read";
    case OPCODE_WRITE:
      return "write";
    default:
      return NULL;
  }
}

static unsigned int instruction_opcode(Word instruction) {
  return (unsigned int) ((instruction >> 28) & 0xFu);
}

static unsigned int instruction_reg(Word instruction, int index) {
  return (unsigned int) ((instruction >> (24 - 4 * index)) & 0xFu);
}

static unsigned int instruction_address(Word instruction) {
  return (unsigned int) (instruction & ADDRESS_MASK_16);
}

static int opcode_uses_address(unsigned int opcode) {
  return opcode == OPCODE_SHL || opcode == OPCODE_LW || opcode == OPCODE_LI ||
         opcode == OPCODE_SW || opcode == OPCODE_BNE;
}

static int opcode_requires_general_destination(unsigned int opcode) {
  return opcode != OPCODE_HALT;
}

static int valid_register_field(unsigned int reg) {
  return reg >= REGISTER_MIN && reg <= REGISTER_MAX;
}

static int valid_general_register(unsigned int reg) {
  return reg >= GENERAL_REGISTER_MIN && reg <= GENERAL_REGISTER_MAX;
}

void print_instruction(Word instruction) {
  unsigned int opcode = instruction_opcode(instruction);
  int operand_count = opcode_operand_count(opcode);
  const char *name = opcode_name(opcode);
  int i;

  if (name == NULL || operand_count < 0) {
    printf("invalid\n");
    return;
  }

  printf("%s", name);

  for (i = 0; i < operand_count; ++i) {
    printf(" R%02u", instruction_reg(instruction, i));
  }

  if (opcode_uses_address(opcode)) {
    printf(" %u", instruction_address(instruction));
  }

  printf("\n");
}

int disassemble(const Word memory[], int starting_address, int num_words) {
  int start_index;
  int printed;

  if (memory == NULL) {
    return -1;
  }

  if (num_words <= 0) {
    return 0;
  }

  if (starting_address < 0 || starting_address % MEMORY_WORD_BYTES != 0) {
    return -1;
  }

  if (starting_address >= MEMORY_CAPACITY_BYTES) {
    return -1;
  }

  start_index = starting_address / MEMORY_WORD_BYTES;
  if (start_index + num_words > MEMORY_CAPACITY_WORDS) {
    return -1;
  }

  for (printed = 0; printed < num_words; ++printed) {
    Word instruction = memory[start_index + printed];

    if (!valid_instruction(instruction)) {
      break;
    }

    printf("0x%04x: ", starting_address + printed * MEMORY_WORD_BYTES);
    print_instruction(instruction);
  }

  return printed;
}

int encode_instruction(unsigned int opcode, unsigned int reg1,
                       unsigned int reg2, unsigned int reg3,
                       unsigned int memory_addr, Word *const instruction) {
  Word encoded;

  if (instruction == NULL) {
    return 0;
  }

  if (opcode_operand_count(opcode) < 0) {
    return 0;
  }

  if (!opcode_uses_address(opcode)) {
    memory_addr = 0;
  }

  switch (opcode) {
    case OPCODE_HALT:
      reg1 = 0;
      reg2 = 0;
      reg3 = 0;
      break;
    case OPCODE_LW:
    case OPCODE_LI:
    case OPCODE_SW:
    case OPCODE_READ:
    case OPCODE_WRITE:
      reg2 = 0;
      reg3 = 0;
      break;
    case OPCODE_NEG:
    case OPCODE_SHL:
    case OPCODE_NOT:
    case OPCODE_MOVE:
    case OPCODE_BNE:
      reg3 = 0;
      break;
    default:
      break;
  }

  encoded = ((Word) opcode << 28) |
            ((Word) reg1 << 24) |
            ((Word) reg2 << 20) |
            ((Word) reg3 << 16) |
            (Word) (memory_addr & ADDRESS_MASK_16);

  if (!valid_instruction(encoded)) {
    return 0;
  }

  *instruction = encoded;
  return 1;
}

int valid_instruction(Word instruction) {
  unsigned int opcode = instruction_opcode(instruction);
  int operand_count = opcode_operand_count(opcode);
  unsigned int reg1 = instruction_reg(instruction, 0);
  int i;

  if (operand_count < 0) {
    return 0;
  }

  for (i = 0; i < operand_count; ++i) {
    if (!valid_register_field(instruction_reg(instruction, i))) {
      return 0;
    }
  }

  if (opcode_requires_general_destination(opcode) && !valid_general_register(reg1)) {
    return 0;
  }

  if (opcode_uses_address(opcode)) {
    unsigned int addr = instruction_address(instruction);

    if (addr % MEMORY_WORD_BYTES != 0) {
      return 0;
    }

    if (opcode == OPCODE_SHL) {
      if ((instruction & SHIFT_IMMEDIATE_MASK) > MAX_SHIFT_AMOUNT) {
        return 0;
      }
    } else if (addr >= MEMORY_CAPACITY_BYTES) {
      return 0;
    }
  }

  return 1;
}
