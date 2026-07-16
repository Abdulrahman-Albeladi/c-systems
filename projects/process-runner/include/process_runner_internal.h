#ifndef PROCESS_RUNNER_INTERNAL_H
#define PROCESS_RUNNER_INTERNAL_H

/*
 * Internal data structures for process-runner command planning.
 *
 * This header intentionally defines only a small reusable concept extracted
 * from the source material: storing argument vectors for multiple command
 * phases. It avoids assignment-specific names and interfaces.
 */

#include <stddef.h>

typedef struct {
  char **build_argv;
  char **run_argv;
  size_t build_argc;
  size_t run_argc;
} ProcessCommandPlan;

#endif /* PROCESS_RUNNER_INTERNAL_H */
