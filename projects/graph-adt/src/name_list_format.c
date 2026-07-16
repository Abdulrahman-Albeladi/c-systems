#include <stddef.h>
#include <string.h>

#include "name_list_format.h"

void name_list_format(char *const names[], char result[], size_t result_capacity) {
  size_t used = 0;
  size_t i = 0;

  if (result == NULL || result_capacity == 0) {
    return;
  }

  result[0] = '\0';

  if (names == NULL) {
    return;
  }

  while (names[i] != NULL) {
    const char *name = names[i];
    size_t name_len;
    size_t required;

    if (name == NULL) {
      break;
    }

    name_len = strlen(name);
    required = name_len + (used > 0 ? 1u : 0u);

    /*
     * Keep the output always NUL-terminated and stop before a partial write.
     * This produces a space-delimited representation suitable for simple
     * logging or deterministic text comparisons in graph-related utilities.
     */
    if (required >= result_capacity - used) {
      break;
    }

    if (used > 0) {
      result[used++] = ' ';
    }

    memcpy(result + used, name, name_len);
    used += name_len;
    result[used] = '\0';

    i++;
  }
}
