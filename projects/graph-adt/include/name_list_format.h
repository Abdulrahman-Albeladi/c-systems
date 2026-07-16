#ifndef NAME_LIST_FORMAT_H
#define NAME_LIST_FORMAT_H

#include <stddef.h>

/*
 * Formats a sequence of names into a caller-provided buffer.
 *
 * Output form:
 *   - empty input: ""
 *   - one name: "name"
 *   - multiple names: "name1, name2, name3"
 *
 * The function always NUL-terminates the destination buffer when
 * destination_size > 0.
 *
 * Parameters:
 *   names            array of pointers to NUL-terminated strings
 *   count            number of elements in names
 *   destination      output buffer
 *   destination_size size of output buffer in bytes
 *
 * Returns:
 *   The number of characters that would have been written excluding the
 *   terminating NUL byte. If the return value is greater than or equal to
 *   destination_size, truncation occurred.
 *
 * Notes:
 *   - NULL name entries are formatted as empty strings.
 *   - Passing names == NULL is valid only when count == 0.
 */
size_t format_name_list(const char *const *names,
                        size_t count,
                        char *destination,
                        size_t destination_size);

#endif
