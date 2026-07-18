#include <assert.h>
#include <stdio.h>

#include "header08.h"

int main(void) {
  assert(power(10, 4) == 10000);

  printf("power(10, 4) returned 10000.\n");
  return 0;
}
