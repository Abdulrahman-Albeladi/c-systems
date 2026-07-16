/*
 * Independent reconstruction note:
 * This header defines a small assembler interface for a machine-simulator
 * project. It is intentionally limited to concept-level module boundaries and
 * does not preserve assignment-specific APIs, tests, or submission scaffolding.
 *
 * Original source material for the private staging repository included
 * collaborator-owned coursework content. Publication should remain private
 * until attribution and release boundaries are reviewed.
 */

#ifndef MACHINE_SIMULATOR_ASSEMBLER_H
#define MACHINE_SIMULATOR_ASSEMBLER_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t ms_word_t;

typedef enum {
  MS_ASSEMBLER_OK = 0,
  MS_ASSEMBLER_IO_ERROR,
  MS_ASSEMBLER_PARSE_ERROR,
  MS_ASSEMBLER_SYMBOL_ERROR,
  MS_ASSEMBLER_CAPACITY_ERROR
} ms_assembler_status_t;

typedef struct {
  ms_assembler_status_t status;
  size_t words_written;
  size_t line_number;
  const char *message;
} ms_assembler_result_t;

/*
 * Assembles source text from an already-open stream into the provided output
 * buffer. The caller owns the stream and output storage.
 */
ms_assembler_result_t ms_assemble_stream(FILE *input,
                                         ms_word_t *output,
                                         size_t output_capacity);

/*
 * Assembles source text from a file path into the provided output buffer.
 * Passing a null path is invalid; use ms_assemble_stream() for stdin or other
 * custom input sources.
 */
ms_assembler_result_t ms_assemble_file(const char *path,
                                       ms_word_t *output,
                                       size_t output_capacity);

#ifdef __cplusplus
}
#endif

#endif