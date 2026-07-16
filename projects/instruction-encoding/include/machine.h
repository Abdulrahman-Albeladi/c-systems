#ifndef INSTRUCTION_ENCODING_MACHINE_H
#define INSTRUCTION_ENCODING_MACHINE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Public interface for a small fixed-width instruction encoding library.
 *
 * This header intentionally exposes a concept-level API rather than preserving
 * any assignment-specific names or signatures from source coursework.
 */

typedef uint32_t ie_word_t;

enum ie_opcode {
  IE_OP_HALT = 0,
  IE_OP_ADD,
  IE_OP_MUL,
  IE_OP_NEG,
  IE_OP_SHL,
  IE_OP_AND,
  IE_OP_NOT,
  IE_OP_LOAD,
  IE_OP_LOAD_IMM,
  IE_OP_STORE,
  IE_OP_MOVE,
  IE_OP_BRANCH_NE,
  IE_OP_READ,
  IE_OP_WRITE
};

enum {
  IE_REGISTER_COUNT = 12
};

typedef struct ie_fields {
  enum ie_opcode opcode;
  uint8_t reg_a;
  uint8_t reg_b;
  uint8_t reg_c;
  uint32_t address;
} ie_fields_t;

/*
 * Encodes a logical instruction description into a machine word.
 * Returns true on success and false when any field is out of range.
 */
bool ie_encode(const ie_fields_t *fields, ie_word_t *out_word);

/*
 * Decodes a machine word into logical fields.
 * Returns true when the word matches the supported encoding.
 */
bool ie_decode(ie_word_t word, ie_fields_t *out_fields);

/*
 * Validates whether a word can be decoded as a supported instruction.
 */
bool ie_is_valid(ie_word_t word);

/*
 * Writes a human-readable representation of one instruction to the provided
 * stream. Returns false if the instruction is invalid or the stream is null.
 */
bool ie_fprint_instruction(FILE *stream, ie_word_t word);

/*
 * Disassembles a contiguous block of words to the provided stream.
 * Returns the number of instructions written. A return value of 0 may also
 * indicate invalid input such as a null stream or null memory pointer.
 */
size_t ie_disassemble(FILE *stream,
                      const ie_word_t *memory,
                      size_t start_index,
                      size_t word_count);

#ifdef __cplusplus
}
#endif

#endif /* INSTRUCTION_ENCODING_MACHINE_H */
