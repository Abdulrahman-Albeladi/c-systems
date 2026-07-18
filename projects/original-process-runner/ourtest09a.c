#include <assert.h>
#include <stdio.h>

#include "header09.h"

int main(void) {
  int a;
  int b;
  int c;

  if (scanf("%d %d %d", &a, &b, &c) != 3) {
    fprintf(stderr, "Expected three integer values.\n");
    return 1;
  }

  assert(max(a, b, c) == 789);

  printf("max returned the expected value.\n");
  return 0;
}
