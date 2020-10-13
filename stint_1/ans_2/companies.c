#include "master_header.h"


// struct company
// {
//     pthread_t thread_obj;
//     int id;
//     int thr_id;
//     int curr_batches_num;
//     int left_batches_num;
//     int capacity_of_batches;
//     long double prob_of_success;
//     pthread_mutex_t mutex;

// };

void* init_company(void *ptr)
{
    int id=*((int *)ptr);
    comp_ptr[id]->capacity_of_batches=0;
    comp_ptr[id]->curr_batches_num=0;
    comp_ptr[id]->left_batches_num=0;
    pthread_mutex_init(&(comp_ptr[id]->mutex), NULL);

}