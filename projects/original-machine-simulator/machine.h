#ifndef MACHINE_H
#define MACHINE_H

/* Instruction opcodes for the simulated machine. */
typedef enum {
    HALT,
    ADD,
    MUL,
    NEG,
    SHL,
    AND,
    NOT,
    LW,
    LI,
    SW,
    MOVE,
    BNE,
    READ,
    WRITE
} Opcode;

enum {
    R00 = 0,
    R01 = 1,
    R02 = 2,
    R03 = 3,
    R04 = 4,
    R05 = 5,
    R06 = 6,
    R07 = 7,
    R08 = 8,
    R09 = 9,
    R10 = 10,
    R11 = 11
};

typedef unsigned int Word;

void print_instruction(Word instruction);
int disassemble(const Word memory[], int starting_address, int num_words);
int encode_instruction(unsigned int opcode, unsigned int reg1,
                       unsigned int reg2, unsigned int reg3,
                       unsigned int memory_addr, Word *const instruction);
int valid_instruction(Word instruction);

#endif /* MACHINE_H */
