#ifndef MACHINE_SIMULATOR_INTERPRETER_H
#define MACHINE_SIMULATOR_INTERPRETER_H

#include <stddef.h>
#include <stdint.h>

/*
 * Independent machine-simulator interface.
 *
 * This header defines a small, assignment-neutral API for loading and running
 * a word-addressed machine model. It intentionally avoids preserving any
 * course-specific names or submission-facing interfaces.
 */

#ifdef __cplusplus
extern "C" {
#endif

enum {
  VM_REGISTER_COUNT = 12,
  VM_MEMORY_WORDS = (1024u * 48u) / 4u
};

typedef uint32_t VmWord;

typedef struct {
  int32_t registers[VM_REGISTER_COUNT];
  VmWord memory[VM_MEMORY_WORDS];
} VmMachine;

typedef enum {
  VM_RUN_HALTED = 0,
  VM_RUN_STEP_LIMIT,
  VM_RUN_INVALID_INSTRUCTION,
  VM_RUN_BAD_ARGUMENT
} VmRunStatus;

/*
 * Clears machine state to a deterministic zeroed configuration.
 * Returns 0 on success and -1 for invalid arguments.
 */
int vm_reset(VmMachine *machine);

/*
 * Copies a program image into machine memory starting at address 0.
 * Existing machine state is reset before loading.
 * Returns 0 on success and -1 for invalid arguments or oversized programs.
 */
int vm_load_program(VmMachine *machine, const VmWord *program, size_t program_words);

/*
 * Executes up to step_limit instructions.
 *
 * executed_steps may be NULL. When non-NULL, it receives the number of
 * instructions attempted during this call.
 *
 * enable_trace is treated as a boolean flag by the implementation.
 */
VmRunStatus vm_run(VmMachine *machine,
                   size_t step_limit,
                   size_t *executed_steps,
                   int enable_trace);

#ifdef __cplusplus
}
#endif

#endif