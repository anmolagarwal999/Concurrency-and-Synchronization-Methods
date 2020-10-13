#ifndef GLOBAL_VARS_HEADER
#define GLOBAL_VARS_HEADER

#include <stdbool.h>

extern int num_hospitals;
extern int num_companies;
extern int num_students;

struct comp
{
    int id;
    int thr_id;
    int curr_batches;
    int left_batches;
    int capacity_of_batches;
    long double prob_of_success;
};

struct hosp
{
    int id;
    int thr_id;

    int partner_company;
    int tot_vaccines;
    int left_vaccines;
    int curr_slots;
    int left_slots;
};

struct stu
{
    int id;
    int thr_id;

    int arrival_time;
    //0-> if yet waiting
    //1-> ongoing
    //2-> successful vaccination
    int curr_stat;
    int rounds_already;
};

#endif
