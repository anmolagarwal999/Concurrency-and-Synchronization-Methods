#include "master_header.h"

struct timespec *get_abs_time_obj(int sec_can_wait)
{
    //https://www.gnu.org/software/libc/manual/html_node/Time-Types.html
    struct timespec *ptr = (struct timespec *)malloc(sizeof(struct timespec));
    // https://stackoverflow.com/q/46018295/
    /* If the thread is preempted between the first statement and the last statement, the thread blocks for too long. Blocking, however, is irrelevant if an absolute timeout is used. 
    An absolute timeout also need not be recomputed if 
    it is used multiple times in a loop, such as that enclosing a condition wait.*/
    clock_gettime(CLOCK_REALTIME, ptr);
    ptr->tv_sec += sec_can_wait;
    return ptr;
}

int get_random_int(int lower, int upper)
{
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

int get_random_int2(int lower, int upper, char *seg_id)
{
    // printf("seg id is %s\n", seg_id);
    // printf("inside ran() stuff\n");
    // printf("Hopeful num left are %d\n", hopeful_students_num);
    // debug(lower);
    // debug(upper);
    // part;
    // fflush(stdout);
    int num = (rand() % (upper - lower + 1)) + lower;

    // if(lower==upper)
    // {
    //     exit(0);
    // }
    // if (lower == 1 && upper == 2)
    //     exit(0);

    return num;
}
