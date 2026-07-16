#include "header09.h"

int max(int x, int y, int z)
{
    int largest = (x > y) ? x : y;

    return (largest > z) ? largest : z;
}
