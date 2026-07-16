#include <assert.h>
#include <stdio.h>

#include "header08.h"

int main(void) {
    assert(power(2, 8) == 256);

    printf("power(2, 8) returned 256.\n");
    return 0;
}
