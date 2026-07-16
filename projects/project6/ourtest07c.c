#include <assert.h>
#include <stdio.h>

#include "header07.h"

int main(void) {
    assert(product(2, 16) == 32);

    printf("product(2, 16) returned 32.\n");
    return 0;
}
