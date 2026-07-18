/* Original implementation: Yufan Fei */

#include "machine.h"

#include <stdio.h>

enum { MEMORY_WORDS = 12288 };

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
  static const char *const mnemonics[] = {
      "halt", "add", "mul", "neg", "shl", "and", "not",
      "lw",   "li",  "sw",  "move", "bne", "read", "write"};
  unsigned int value = (unsigned int)instruction;
  unsigned int opcode = value >> 28;
  int operands = operand_count(opcode);
  int index;

  if (operands < 0) {
    printf("\n");
    return;
  }

  printf("%s ", mnemonics[opcode]);

  for (index = 0; index < operands; index++) {
    printf(" R%02u", (value >> (24 - 4 * index)) & 0x0fU);
  }

  if (opcode == 4 || opcode == 7 || opcode == 8 || opcode == 9 ||
      opcode == 11) {
    printf(" %05u", value & 0xffffU);
  }

  printf("\n");
}

int disassemble(const Word memory[], int starting_address, int num_words) {
  int start_index;
  int num_printed;

  if (memory == NULL || starting_address < 0 ||
      starting_address % 4 != 0) {
    return -1;
  }

  if (num_words <= 0) {
    return 0;
  }

  start_index = starting_address / 4;
  if (start_index >= MEMORY_WORDS ||
      num_words > MEMORY_WORDS - start_index) {
    return -1;
  }

  for (num_printed = 0;
       num_printed < num_words &&
       valid_instruction(memory[start_index + num_printed]);
       num_printed++) {
    printf("0x%04x: ", starting_address + 4 * num_printed);
    print_instruction(memory[start_index + num_printed]);
  }

  return num_printed;
}

int encode_instruction(unsigned int opcode, unsigned int reg1,
                       unsigned int reg2, unsigned int reg3,
                       unsigned int memory_addr, Word *const instruction) {
  unsigned int encoded_instruction;

  if (instruction == NULL || operand_count(opcode) < 0) {
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

  if (reg1 > 0x0fU || reg2 > 0x0fU || reg3 > 0x0fU ||
      memory_addr > 0xffffU) {
    return 0;
  }

  encoded_instruction = opcode << 28;
  encoded_instruction |= reg1 << 24;
  encoded_instruction |= reg2 << 20;
  encoded_instruction |= reg3 << 16;
  encoded_instruction |= memory_addr;

  if (!valid_instruction((Word)encoded_instruction)) {
    return 0;
  }

  *instruction = (Word)encoded_instruction;
  return 1;
}

int valid_instruction(Word instruction) {
  unsigned int value = (unsigned int)instruction;
  unsigned int opcode = value >> 28;
  unsigned int memory_addr = value & 0xffffU;
  int operands = operand_count(opcode);
  int index;

  if (operands < 0) {
    return 0;
  }

  for (index = 0; index < operands; index++) {
    if (((value >> (24 - 4 * index)) & 0x0fU) > 11U) {
      return 0;
    }
  }

  if (opcode != 0) {
    unsigned int reg1 = (value >> 24) & 0x0fU;

    if (reg1 == 0 || reg1 == 11) {
      return 0;
    }
  }

  if (opcode == 4 || opcode == 7 || opcode == 8 || opcode == 9 ||
      opcode == 11) {
    if (memory_addr > 49151U || memory_addr % 4 != 0) {
      return 0;
    }
  }

  if (opcode == 4 && memory_addr > 31U) {
    return 0;
  }

  return 1;
}
