#include "machine.h"

#include <stdio.h>

/*
 * Independent reconstruction of a small word-encoded instruction utility.
 *
 * Encoding layout:
 *   bits 31..28: opcode
 *   bits 27..24: reg1
 *   bits 23..20: reg2
 *   bits 19..16: reg3
 *   bits 15..0 : immediate/address field
 *
 * This implementation keeps the observable behavior implied by the source:
 * printing decoded instructions, disassembling a memory range, encoding a
 * validated instruction word, and validating instruction fields.
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
  OPCODE_MAX = 13
};

enum {
  REGISTER_MIN = 0,
  REGISTER_MAX = 11,
  MEMORY_WORD_ALIGNMENT = 4,
  MEMORY_MAX_ADDRESS = 49151,
  MEMORY_CAPACITY_WORDS = 12288,
  SHIFT_IMMEDIATE_MAX = 31
};

static unsigned int get_opcode(Word instruction) {
  return (unsigned int)(instruction >> 28);
}

static unsigned int get_register_field(Word instruction, int index) {
  return (unsigned int)((instruction >> (24 - 4 * index)) & 0xFu);
}

static unsigned int get_low_16_bits(Word instruction) {
  return (unsigned int)(instruction & 0xFFFFu);
}

static int operand_count(unsigned int opcode) {
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
    case OPCODE_HALT:  return "halt ";
    case OPCODE_ADD:   return "add";
    case OPCODE_MUL:   return "mul";
    case OPCODE_NEG:   return "neg";
    case OPCODE_SHL:   return "shl ";
    case OPCODE_AND:   return "and ";
    case OPCODE_NOT:   return "not ";
    case OPCODE_LW:    return "lw  ";
    case OPCODE_LI:    return "li  ";
    case OPCODE_SW:    return "sw  ";
    case OPCODE_MOVE:  return "move";
    case OPCODE_BNE:   return "bne ";
    case OPCODE_READ:  return "read ";
    case OPCODE_WRITE: return "write ";
    default:           return "";
  }
}

static int opcode_uses_memory_field(unsigned int opcode) {
  return opcode == OPCODE_SHL || opcode == OPCODE_LW || opcode == OPCODE_LI ||
         opcode == OPCODE_SW || opcode == OPCODE_BNE;
}

static int opcode_requires_aligned_memory_address(unsigned int opcode) {
  return opcode == OPCODE_LW || opcode == OPCODE_SW || opcode == OPCODE_BNE;
}

void print_instruction(Word instruction) {
  unsigned int opcode = get_opcode(instruction);
  int count = operand_count(opcode);
  int i;

  printf("%s", opcode_name(opcode));

  for (i = 0; i < count; i++) {
    printf(" R%02u", get_register_field(instruction, i));
  }

  if (opcode_uses_memory_field(opcode)) {
    printf(" %05u", get_low_16_bits(instruction));
  }

  printf("\n");
}

int disassemble(const Word memory[], int starting_address, int num_words) {
  int i;
  int start_index;

  if (memory == NULL) {
    return -1;
  }

  if (starting_address < 0 || starting_address % MEMORY_WORD_ALIGNMENT != 0) {
    return -1;
  }

  start_index = starting_address / MEMORY_WORD_ALIGNMENT;

  if (start_index < 0 || start_index >= MEMORY_CAPACITY_WORDS) {
    return -1;
  }

  if (num_words <= 0) {
    return 0;
  }

  if (start_index + num_words > MEMORY_CAPACITY_WORDS || start_index + num_words < 0) {
    return -1;
  }

  for (i = 0; i < num_words; i++) {
    Word instruction = memory[start_index + i];

    if (!valid_instruction(instruction)) {
      break;
    }

    printf("0x%04x: ", starting_address + MEMORY_WORD_ALIGNMENT * i);
    print_instruction(instruction);
  }

  return i;
}

int encode_instruction(unsigned int opcode, unsigned int reg1,
                       unsigned int reg2, unsigned int reg3,
                       unsigned int memory_addr, Word *const instruction) {
  Word encoded = 0;

  if (instruction == NULL) {
    return 0;
  }

  if (opcode == OPCODE_HALT) {
    reg1 = 0;
  }

  if (opcode == OPCODE_HALT || opcode == OPCODE_LW || opcode == OPCODE_LI ||
      opcode == OPCODE_SW || opcode == OPCODE_READ || opcode == OPCODE_WRITE) {
    reg2 = 0;
  }

  if (opcode != OPCODE_ADD && opcode != OPCODE_MUL && opcode != OPCODE_AND) {
    reg3 = 0;
  }

  if (!opcode_uses_memory_field(opcode)) {
    memory_addr = 0;
  }

  encoded |= (Word)(opcode << 28);
  encoded |= (Word)(reg1 << 24);
  encoded |= (Word)(reg2 << 20);
  encoded |= (Word)(reg3 << 16);
  encoded |= (Word)(memory_addr & 0xFFFFu);

  if (!valid_instruction(encoded)) {
    return 0;
  }

  *instruction = encoded;
  return 1;
}

int valid_instruction(Word instruction) {
  unsigned int opcode = get_opcode(instruction);
  unsigned int reg1 = get_register_field(instruction, 0);
  unsigned int field16 = get_low_16_bits(instruction);
  int count;
  int i;

  if (opcode > OPCODE_MAX) {
    return 0;
  }

  count = operand_count(opcode);
  if (count < 0) {
    return 0;
  }

  for (i = 0; i < count; i++) {
    if (get_register_field(instruction, i) < REGISTER_MIN ||
        get_register_field(instruction, i) > REGISTER_MAX) {
      return 0;
    }
  }

  if (opcode_requires_aligned_memory_address(opcode)) {
    if (field16 > MEMORY_MAX_ADDRESS || field16 % MEMORY_WORD_ALIGNMENT != 0) {
      return 0;
    }
  }

  if (opcode != OPCODE_HALT && opcode != OPCODE_SW && opcode != OPCODE_BNE &&
      opcode != OPCODE_WRITE && (reg1 == 0 || reg1 == 11)) {
    return 0;
  }

  if (opcode == OPCODE_SHL && field16 > SHIFT_IMMEDIATE_MAX) {
    return 0;
  }

  return 1;
}
