#include <stddef.h>
#include <string.h>

#include "name_list_format.h"

size_t name_list_format(const char *const names[],
                        char *destination,
                        size_t destination_size) {
  size_t used = 0;
  size_t i = 0;

  if (destination == NULL || destination_size == 0) {
    return 0;
  }

  destination[0] = '\0';

  if (names == NULL) {
    return 0;
  }

  while (names[i] != NULL) {
    const char *name = names[i];
    size_t name_len = (name != NULL) ? strlen(name) : 0u;
    size_t required = name_len + (used > 0 ? 1u : 0u); /* space + name */

    /* Ensure room for delimiter+name and trailing NUL without truncation. */
    if (required >= destination_size - used) {
      break;
    }

    if (used > 0) {
      destination[used++] = ' ';
    }

    if (name_len > 0) {
      memcpy(destination + used, name, name_len);
      used += name_len;
    }

    destination[used] = '\0';
    ++i;
  }

  return used;
}
