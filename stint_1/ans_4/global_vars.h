#ifndef GLOBAL_VARS_HEADER
#define GLOBAL_VARS_HEADER

#include <stdbool.h>

//https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c

  //solo by musician
  #define ANSI_RED "\033[1;31m"    

//solo for singer
#define ANSI_GREEN "\033[1;32m" 

// for arrival
#define ANSI_YELLOW "\033[1;33m" 

  //singer joins duel
#define ANSI_BLUE "\x1b[34m"

//t-shirt collection
#define ANSI_MAGENTA "\x1b[35m" 

  //performance ending
#define ANSI_CYAN "\033[1;36m"
#define ANSI_RESET "\x1b[0m"

//Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"


enum performer_types
{
    perf_a,
    perf_e,
    perf_ae,
    perf_s
};

enum stage_statuses
{
    Unoccupied,
    one_occupant,
    two_folks
};

enum stage_types
{
    TYPE_A,
    TYPE_E
};

#define stage_type_a 1
#define stage_type_e 2

extern int tot_num_performers;
extern int nump_a, nump_e, nump_ae, nump_s;
extern int t1, t2, patience_time;
extern int num_stage_a, num_stage_e;
extern int tot_num_stages;
extern int num_coordinators;

extern sem_t sem_empty_a, sem_empty_e, sem_filled_ae;
extern sem_t rogue_sem, sem_tshirt_givers;

enum performer_statuses
{
    Unarrived,
    Waiting,
    Performing_solo,
    open_to_duel,
    Performing_duel,
    Wait_for_shirt,
    Collecting_shirt,
    Left_show
};

struct performer
{
    char name[100];
    int id;
    enum performer_types type;
    int arrival_time;
    char instrument_id;
    int stage_allotted;
    int perf_time;
    pthread_mutex_t mutex;
    pthread_cond_t cv;
    pthread_t thread_obj[3];
    int thr_id[3];
    enum performer_statuses curr_stat;
    struct timespec * st_arrival,*st_leave;
};

struct stage
{
    int stage_id;
    int perf_id1;
    int perf_id2;
    enum stage_types type;
    int curr_stat;
    pthread_mutex_t mutex;
};

#define max_inp_to_entities 500

struct performer *perf_ptr[max_inp_to_entities];
struct stage *st_ptr[max_inp_to_entities];

#endif
