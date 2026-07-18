#include <stdio.h>

#define CAPACITY 100

int main(void) {
    int values[CAPACITY];
    int count = 0;
    int value;

    while (scanf("%d", &value) == 1 && value != 0) {
        if (count < CAPACITY) {
            values[count++] = value;
        }
    }

    while (scanf("%d", &value) == 1 && value != 0) {
        int index = 0;

        while (index < count && values[index] != value) {
            index++;
        }

        printf("%d\n", index < count ? index : -1);
    }

    return 0;
}
