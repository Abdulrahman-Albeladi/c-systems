#include "machine.h"

#include <stdio.h>

enum {
  MEMORY_WORDS = 12288,
  MAX_OPCODE = 13,
  MAX_REGISTER = 15,
  MAX_MEMORY_ADDRESS = 0xffff
};

static int operand_count(unsigned int opcode) {
  switch (opcode) {
    case 0:
      return 0;
    case 1:
    case 2:
    case 5:
      return 3;
    case 3:
    case 4:
    case 6:
    case 10:
    case 11:
      return 2;
    case 7:
    case 8:
    case 9:
    case 12:
    case 13:
      return 1;
    default:
      return -1;
  }
}

void print_instruction(Word instruction) {
  unsigned int bits = (unsigned int)instruction;
  unsigned int opcode = bits >> 28;
  int operands = operand_count(opcode);
  int index;

  switch (opcode) {
    case 0:
      printf("halt ");
      break;
    case 1:
      printf("add");
      break;
    case 2:
      printf("mul");
      break;
    case 3:
      printf("neg");
      break;
    case 4:
      printf("shl ");
      break;
    case 5:
      printf("and ");
      break;
    case 6:
      printf("not ");
      break;
    case 7:
      printf("lw  ");
      break;
    case 8:
      printf("li  ");
      break;
    case 9:
      printf("sw  ");
      break;
    case 10:
      printf("move");
      break;
    case 11:
      printf("bne ");
      break;
    case 12:
      printf("read ");
      break;
    case 13:
      printf("write ");
      break;
    default:
      break;
  }

  for (index = 0; index < operands; index++) {
    printf(" R%02u", (bits >> (24 - 4 * index)) & 0xf);
  }

  if (opcode == 4 || opcode == 7 || opcode == 8 || opcode == 9 ||
      opcode == 11) {
    printf(" %05u", bits & 0xffff);
  }

  printf("\n");
}

int disassemble(const Word memory[], int starting_address, int num_words) {
  int start_word;
  int index;

  if (memory == NULL || starting_address < 0 ||
      starting_address % 4 != 0) {
    return -1;
  }

  start_word = starting_address / 4;
  if (start_word >= MEMORY_WORDS) {
    return -1;
  }

  if (num_words <= 0) {
    return 0;
  }

  if (num_words > MEMORY_WORDS - start_word) {
    return -1;
  }

  for (index = 0; index < num_words && valid_instruction(memory[start_word + index]);
       index++) {
    printf("0x%04x: ", starting_address + 4 * index);
    print_instruction(memory[start_word + index]);
  }

  return index;
}

int encode_instruction(unsigned int opcode, unsigned int reg1,
                       unsigned int reg2, unsigned int reg3,
                       unsigned int memory_addr, Word *const instruction) {
  unsigned int encoded;

  if (instruction == NULL || opcode > MAX_OPCODE) {
    return 0;
  }

  if (opcode == 0) {
    reg1 = 0;
  }

  if (opcode == 0 || opcode == 7 || opcode == 8 || opcode == 9 ||
      opcode == 12 || opcode == 13) {
    reg2 = 0;
  }

  if (opcode != 1 && opcode != 2 && opcode != 5) {
    reg3 = 0;
  }

  if (opcode != 4 && opcode != 7 && opcode != 8 && opcode != 9 &&
      opcode != 11) {
    memory_addr = 0;
  }

  if (reg1 > MAX_REGISTER || reg2 > MAX_REGISTER || reg3 > MAX_REGISTER ||
      memory_addr > MAX_MEMORY_ADDRESS) {
    return 0;
  }

  encoded = opcode << 28;
  encoded |= reg1 << 24;
  encoded |= reg2 << 20;
  encoded |= reg3 << 16;
  encoded |= memory_addr;

  if (!valid_instruction((Word)encoded)) {
    return 0;
  }

  *instruction = (Word)encoded;
  return 1;
}

int valid_instruction(Word instruction) {
  unsigned int bits = (unsigned int)instruction;
  unsigned int opcode = bits >> 28;
  unsigned int reg1 = (bits >> 24) & 0xf;
  int operands = operand_count(opcode);

  if (operands < 0) {
    return 0;
  }

  while (operands > 0) {
    if (((bits >> (28 - operands * 4)) & 0xf) > 11) {
      return 0;
    }
    operands--;
  }

  if ((opcode == 7 || opcode == 9 || opcode == 11) &&
      ((bits & 0xffff) > 49151 || (bits & 0x3) != 0)) {
    return 0;
  }

  if (opcode != 0 && opcode != 9 && opcode != 11 && opcode != 13 &&
      (reg1 == 0 || reg1 == 11)) {
    return 0;
  }

  if (opcode == 4 && (bits & 0xffff) > 31) {
    return 0;
  }

  return 1;
}
