#include "master_header.h"

// struct student
// {
//     pthread_t thread_obj;

//     int id;
//     int thr_id;

//     int arrival_time;
//     //0-> if yet waiting
//     //1-> ongoing
//     //2-> successful vaccination
//     int curr_stat;
//     int rounds_already;
//     pthread_mutex_t mutex;

// };

void seek_hospital(int id)
{
    
}

void *init_student(void *ptr)
{
    int id = *((int *)ptr);

    int late_time = get_random_int(0, latest_arrival_permitted);
    stu_ptr[id]->arrival_time = late_time;
    //stu_ptr[id]->curr_stat = 0;
    stu_ptr[id]->rounds_already = 0;

    if (late_time != 0)
    {
        sleep(late_time);
    }
    pthread_mutex_init(&(stu_ptr[id]->mutex), NULL);
    seek_hospital(id);

    return NULL;
}