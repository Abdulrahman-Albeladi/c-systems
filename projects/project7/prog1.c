#include <stdio.h>

int main(void) {
    int start;
    int end;
    int divisor;
    int count = 0;

    if (scanf("%d %d %d", &start, &end, &divisor) != 3 || divisor == 0) {
        return 1;
    }

    while (start <= end) {
        if (start % divisor == 0) {
            count++;
        }
        start++;
    }

    printf("%d\n", count);
    return 0;
}
