#ifndef GLOBAL_VARS_HEADER
#define GLOBAL_VARS_HEADER

#include <stdbool.h>

//https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define ANSI_RED "\033[1;31m"
#define ANSI_GREEN "\033[1;32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN "\x1b[36m"
#define ANSI_RESET "\x1b[0m"

#define ptype_a 1
#define ptype_e 2
#define ptype_ae 3
#define ptype_s 4

#define stage_a 1
#define stage_e 2

extern int num_performers;
extern int num_a, num_e, num_ae, num_s;
extern int t1, t2, patience_time;
extern int num_e_stages;
extern int num_a_stages;
extern int tot_stages;

extern sem_t sem_a, sem_e, sem_ae, sem_s;

struct performer
{
    char perf_name[50];
    int id;
    int perf_type;
    int arrival_time;
    char instrument_id;
    int stage_of_perf;
    int perf_time;
    pthread_mutex_t mutex;
    pthread_cond_t cv;
};

struct stage
{
    int stage_id;
    int perf_id1;
    int perf_id2;
    int stage_type;
    int curr_stat;
};

#endif
