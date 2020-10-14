#ifndef GLOBAL_VARS_HEADER
#define GLOBAL_VARS_HEADER

#include <stdbool.h>

extern int num_hospitals;
extern int num_companies;
extern int num_students;
extern int tot_conclusions_left;
extern int hopeful_students_num;
extern pthread_mutex_t conclusions_mutex;
extern pthread_mutex_t hopeful_mutex;

extern int latest_arrival_permitted;

struct company
{
    pthread_t thread_obj;
    int id;
    int thr_id;
    int curr_batches_num;
    int left_batches_num;
    int done_batches;
    int capacity_of_batches;
    long double prob_of_success;
    pthread_mutex_t mutex;
    pthread_cond_t cv;
};

struct hospital
{
    pthread_t thread_obj;

    int id;
    int thr_id;

    int partner_company;
    int tot_vaccines;
    int left_vaccines;
    int curr_slots;
    int left_slots;
    pthread_mutex_t mutex;
    int curr_served_students[10];
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
    int rounds_already;
    int vaccine_comp_id;
    pthread_mutex_t mutex;
};

#define max_inp_to_entities 500

struct company *comp_ptr[max_inp_to_entities];
struct hospital *hosp_ptr[max_inp_to_entities];
struct student *stu_ptr[max_inp_to_entities];

#endif
