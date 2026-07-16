#include <assert.h>
#include <stdio.h>

#include "header08.h"

int main(void) {
  assert(power(6, 0) == 1);

  printf("power(6, 0) returned 1.\n");
  return 0;
}
