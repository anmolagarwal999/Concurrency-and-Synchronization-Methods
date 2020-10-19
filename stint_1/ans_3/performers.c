#include "master_header.h"
#include "performers.h"



void *performer_entry(void *ptr)
{
    int id = *((int *)ptr);

    //wait for arrival time and then start
    debug(id);
    printf(ANSI_BLUE "Proposed arrival time is %d\n" ANSI_RESET, perf_ptr[id]->arrival_time);
    sleep(perf_ptr[id]->arrival_time);
    perf_ptr[id]->perf_time = get_random_int2(t1, t2, "Pegasus");
    seek_stage(id);
    return NULL;
}