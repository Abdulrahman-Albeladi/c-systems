#include <string.h>

#include "name-list-to-string.h"

/*
 * Joins a NULL-terminated list of names into a space-separated string.
 * Names that would exceed MAX_LEN are omitted.
 */
void name_list_to_string(char **names, char result[]) {
  int i = 0;

  strcpy(result, "");

  while (names[i] != NULL) {
    if (strlen(result) + strlen(names[i]) + 1 < MAX_LEN) {
      strcat(result, names[i]);

      if (names[i + 1] != NULL) {
        strcat(result, " ");
      }
    }

    i++;
  }
}
