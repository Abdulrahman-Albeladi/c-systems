#ifndef MACHINE_H
#define MACHINE_H

/*
 * Minimal public interface for a small word-oriented machine encoding module.
 *
 * This header is an independent reconstruction of the underlying concept:
 * representing instructions as machine words, validating encodings, and
 * providing basic encode/disassemble helpers. It intentionally avoids
 * assignment-specific naming, scaffolding, and register macros.
 */

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Fixed-width instruction storage type used by the simulator and tools. */
typedef uint32_t MachineWord;

/*
 * Supported operations for the reconstructed teaching machine.
 * Numeric values are stable so encoded words remain portable across modules.
 */
typedef enum MachineOpcode {
    MACHINE_OP_HALT = 0,
    MACHINE_OP_ADD,
    MACHINE_OP_MUL,
    MACHINE_OP_NEG,
    MACHINE_OP_SHL,
    MACHINE_OP_AND,
    MACHINE_OP_NOT,
    MACHINE_OP_LOAD_WORD,
    MACHINE_OP_LOAD_IMMEDIATE,
    MACHINE_OP_STORE_WORD,
    MACHINE_OP_MOVE,
    MACHINE_OP_BRANCH_NOT_EQUAL,
    MACHINE_OP_READ,
    MACHINE_OP_WRITE
} MachineOpcode;

enum {
    MACHINE_REGISTER_COUNT = 12
};

/*
 * Encodes one instruction word.
 *
 * Returns 1 on success and 0 when any field is out of range or the output
 * pointer is NULL.
 */
int machine_encode_instruction(unsigned int opcode,
                               unsigned int reg1,
                               unsigned int reg2,
                               unsigned int reg3,
                               unsigned int memory_addr,
                               MachineWord *instruction_out);

/* Returns 1 when the word is a recognized instruction encoding, else 0. */
int machine_valid_instruction(MachineWord instruction);

/* Writes a human-readable representation of one instruction to stdout. */
void machine_print_instruction(MachineWord instruction);

/*
 * Disassembles up to word_count words starting at start_address.
 *
 * Returns the number of words processed, or a negative value on invalid input.
 */
int machine_disassemble(const MachineWord memory[],
                        int start_address,
                        int word_count);

#ifdef __cplusplus
}
#endif

#endif /* MACHINE_H */
