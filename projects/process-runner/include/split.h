#ifndef PROCESS_RUNNER_SPLIT_H
#define PROCESS_RUNNER_SPLIT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Tokenizes a command line into a NULL-terminated argv-style array.
 *
 * Parsing rules:
 * - Splits on ASCII whitespace.
 * - Preserves text inside single or double quotes.
 * - Supports backslash escaping both inside and outside quotes.
 *
 * Memory ownership:
 * - Returns a heap-allocated array of heap-allocated strings.
 * - The caller must release the result with split_free().
 *
 * Error handling:
 * - Returns NULL on allocation failure or malformed input such as an
 *   unterminated quoted string.
 */
char **split_command_line(const char *line);

/* Frees a token array returned by split_command_line(). */
void split_free(char **tokens);

/* Counts tokens in a NULL-terminated token array. Returns 0 for NULL. */
size_t split_count(char *const *tokens);

#ifdef __cplusplus
}
#endif

#endif