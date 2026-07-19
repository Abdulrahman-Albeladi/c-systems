#ifndef NAME_LIST_FORMAT_H
#define NAME_LIST_FORMAT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Joins a NULL-terminated sequence of names into a single string.
 *
 * Output format:
 *   - empty input: ""
 *   - one name: "name"
 *   - multiple names: "name1 name2 name3"   (single-space delimiter)
 *
 * Behavior:
 *   - Always NUL-terminates destination when destination_size > 0.
 *   - Never writes a partial delimiter or partial name; if the next token
 *     does not fully fit (including its preceding space when needed), the
 *     function stops before it.
 *   - names may be NULL to represent an empty sequence. The array itself must
 *     be terminated by a NULL pointer.
 *
 * Returns:
 *   The number of characters actually written to destination, not including
 *   the terminating NUL byte. Returns 0 when destination is NULL or
 *   destination_size == 0.
 */
size_t name_list_format(const char *const names[],
                        char *destination,
                        size_t destination_size);

#ifdef __cplusplus
}
#endif

#endif /* NAME_LIST_FORMAT_H */
