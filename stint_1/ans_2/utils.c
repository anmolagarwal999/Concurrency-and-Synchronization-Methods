#include "master_header.h"

int get_random_int(int lower, int upper)
{

    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}


