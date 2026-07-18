#include <stdio.h>

#include "machine.h"

int main(void) {
  Word instructions[12288] = {
      0x01230000, 0x11230000, 0x21230000, 0x31230000,
      0x41230020, 0x51230000, 0x61230000, 0x71230000,
      0x81230000, 0x91230000, 0xa1230000, 0xb1230000,
      0xc1230000, 0xd1230000,
  };
  int printed;

  instructions[12280] = 0x11230000;
  instructions[12281] = 0x21230000;

  printed = disassemble(instructions, 49252, -1000);
  printf("number printed %d\n", printed);

  return 0;
}
