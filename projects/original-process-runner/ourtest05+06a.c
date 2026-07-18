#include <assert.h>
#include <stdio.h>

#include "header05+06.h"

int main(void) {
  assert(sum(216, 250) == 466);

  printf("sum(216, 250) returned 466.\n");
  return 0;
}
