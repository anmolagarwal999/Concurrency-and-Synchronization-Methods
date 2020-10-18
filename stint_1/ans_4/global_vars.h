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


enum performer_types{
    perf_a,perf_e,perf_ae,perf_s
};


#define stage_type_a 1
#define stage_type_e 2

extern int tot_num_performers;
extern int nump_a, nump_e, nump_ae, nump_s;
extern int t1, t2, patience_time;
extern int num_stage_a,num_stage_e;
extern int tot_num_stages;
extern int num_coordinators;

extern sem_t sem_a, sem_e, sem_ae, sem_s;
extern sem_t sem_a_ae_s,sem_e_ae_s;
extern sem_t rogue_sem,sem_tshirt_givers;

enum performer_statuses{ Unarrived,
                    Waiting,
                    Performing_solo,
                    Performing_duel,
                    Wait_for_shirt,
                    Collecting_shirt,
                    Left_show };

struct performer
{
    char name[100];
    int id;
    enum performer_types type;
    int arrival_time;
    char instrument_id;
    int stage_allotted;
    int perf_time;
    pthread_mutex_t mutex,mutex2;
    pthread_cond_t cv;
    pthread_t thread_obj;
    int thr_id;
    enum performer_statuses curr_stat;
    
};

struct stage
{
    int stage_id;
    int perf_id1;
    int perf_id2;
    int type;
    int curr_stat;
    pthread_t thread_obj;
    int thr_id;
};

#define max_inp_to_entities 500

struct performer *perf_ptr[max_inp_to_entities];
struct stage *st_ptr[max_inp_to_entities];

struct performer *musc_a_ptr[max_inp_to_entities];
struct performer *musc_e_ptr[max_inp_to_entities];
struct performer *musc_ae_ptr[max_inp_to_entities];
struct performer *singer_ptr[max_inp_to_entities];

#endif
