#ifndef PROJECT3_INTERPRETER_H
#define PROJECT3_INTERPRETER_H

#define NUM_REGISTERS 12
#define MAX_MEMORY ((1024 * 48) / 4)

/* Word must be defined by the instruction-format header included first. */
typedef struct {
  int registers[NUM_REGISTERS];
  Word memory[MAX_MEMORY];
} Machine;

typedef enum {
  TIMEOUT,
  HALTED,
  INVALID_INSTRUCTION,
  PARAMETER_ERROR
} Status;

int load_program(Machine *const spim, const Word program[], int program_size);
Status run_SPIM_program(Machine *const spim, int max_instr,
                        int *const num_instr, int trace_flag);
int reset(Machine *const spim);

#endif /* PROJECT3_INTERPRETER_H */
