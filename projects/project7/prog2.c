#include <limits.h>
#include <stdio.h>

static int count_digits(int value);

int main(void) {
  int value;

  if (scanf("%d", &value) != 1) {
    return 1;
  }

  printf("%d\n", count_digits(value));
  return 0;
}

static int count_digits(int value) {
  int digits = 0;

  /* Avoid negating INT_MIN, which is not representable as an int. */
  unsigned int magnitude = (value < 0)
      ? (unsigned int)(-(value + 1)) + 1U
      : (unsigned int)value;

  do {
    magnitude /= 10U;
    ++digits;
  } while (magnitude > 0U);

  return digits;
}
