#include "master_header.h"

int get_random_int(int lower, int upper)
{
    printf("inside ran() stuff\n");
    printf("Hopeful num left are %d\n",hopeful_students_num);
    debug(lower);
    debug(upper);
    fflush(stdout);
    int num = (rand() % (upper - lower + 1)) + lower;

    // if(lower==upper)
    // {
    //     exit(0);
    // }

    return num;
}

