#include "master_header.h"


// struct hospital
// {
//     pthread_t thread_obj;

//     int id;
//     int thr_id;

//     int partner_company;
//     int tot_vaccines;
//     int left_vaccines;
//     int curr_slots;
//     int left_slots;
//     pthread_mutex_t mutex;

// };


void* init_hospitals(void *ptr)
{
    int id=*((int *)ptr);
    hosp_ptr[id]->curr_slots=0;
    hosp_ptr[id]->left_slots=0;
    hosp_ptr[id]->left_vaccines=0;
    hosp_ptr[id]->partner_company=-1;
    hosp_ptr[id]->tot_vaccines=0;
    pthread_mutex_init(&(hosp_ptr[id]->mutex), NULL);


    
    return NULL;
}