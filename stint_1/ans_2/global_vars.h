#ifndef GLOBAL_VARS_HEADER
#define GLOBAL_VARS_HEADER

#include <stdbool.h>

//https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define ANSI_RED     "\033[1;31m"
#define ANSI_GREEN  "\033[1;32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"
#define ANSI_RESET   "\x1b[0m"

//Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"  //positive
#define BGRN "\e[1;32m"   /// stu start wait
#define BYEL "\e[1;33m"   /// round of vaccination invite
#define BBLU "\e[1;34m"  //neg
#define BMAG "\e[1;35m"  ///student allotted a slot
#define BCYN "\e[1;36m"  //entering vaccination phase
#define BWHT "\e[1;37m"

extern int num_hospitals;
extern int num_companies;
extern int num_students;
extern int tot_conclusions_left;
extern int hopeful_students_num;
extern pthread_mutex_t conclusions_mutex;
extern pthread_mutex_t hopeful_mutex;
extern int w_min_prepare_time,w_max_prepare_time;
extern int r_min_val,r_max_val,p_min_val,p_max_val;

extern int latest_arrival_permitted;

struct company
{
    pthread_t thread_obj;
    int id; //Company id
    int thr_id;
    int curr_batches_num; //batches produced in the last manufacture stint
    int left_batches_num;  //batches yet to be gully consumed
    int done_batches;  //batches which have been fully consumed
    int capacity_of_batches;  //number of vaccines in each batch of the current manufacture stint
    long double prob_of_success;
    pthread_mutex_t mutex;
    pthread_cond_t cv;
};

struct hospital
{
    pthread_t thread_obj;

    int id;
    int thr_id;

    int partner_company;  //comapany whose batch currently using
    int tot_vaccines;  //tot vaccines stocked
    int left_vaccines;  //vaccines left in current stock
    int curr_slots;  //current number of doctors present to administer the vaccine
    int left_slots;  //doctors waiting for patient
    pthread_mutex_t mutex;
    int curr_served_students[10]; //ids of students part of current vaccination phase
};

struct student
{
    pthread_t thread_obj;

    int id;
    int thr_id;

    int arrival_time;
    //-1-> hasn't yet arrived
    //0-> if yet waiting
    //1-> ongoing
    //2-> successful vaccination
    int curr_stat;
    int rounds_already;  //number of vaccines already given
    int vaccine_comp_id; //brand of vaccine he received
    pthread_mutex_t mutex;
};

#define max_inp_to_entities 500

struct company *comp_ptr[max_inp_to_entities];
struct hospital *hosp_ptr[max_inp_to_entities];
struct student *stu_ptr[max_inp_to_entities];

#endif
