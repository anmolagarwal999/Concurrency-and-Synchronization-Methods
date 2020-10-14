#include "master_header.h"

int get_random_int(int lower, int upper)
{

    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

void red_color()
{
    printf("\033[1;31m");
}

void yellow_color()
{
    printf("\033[1;33m");
}

void reset_color()
{
    printf("\033[0m");
}


void cyan_color()
{
    printf("\x1b[33m");
}

void green_color()
{
    printf("\033[1;32m");
}

