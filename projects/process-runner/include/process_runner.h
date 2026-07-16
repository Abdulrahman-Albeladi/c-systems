#ifndef PROCESS_RUNNER_H
#define PROCESS_RUNNER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Independent process-runner interface reconstructed from the general concept
 * of preparing and executing command sequences. This header intentionally does
 * not preserve assignment-specific names or interfaces.
 */

typedef struct {
    char **argv;
    size_t argc;
} pr_command;

typedef struct {
    pr_command *items;
    size_t count;
} pr_command_list;

/*
 * Parses a newline-delimited command specification into an owned command list.
 * Each non-empty line is treated as one command. Implementations may support
 * simple shell-like tokenization, but callers should not assume shell
 * expansion.
 *
 * Returns 0 on success and non-zero on failure.
 */
int pr_command_list_parse(const char *text, pr_command_list *out_list);

/* Releases all memory owned by a command list. Safe to call on zeroed values. */
void pr_command_list_free(pr_command_list *list);

/*
 * Executes each command in order.
 *
 * Returns 0 only when all commands complete successfully. A non-zero return
 * indicates a parse, spawn, wait, or child-process failure.
 */
int pr_run_all(const pr_command_list *list);

/*
 * Convenience helper that parses and executes a command specification in one
 * step.
 */
int pr_run_text(const char *text);

#ifdef __cplusplus
}
#endif

#endif
