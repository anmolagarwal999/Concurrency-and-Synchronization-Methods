#include "master_header.h"

// struct student
// {
//     pthread_t thread_obj;

//     int id;
//     int thr_id;

//     int arrival_time;
//     //0-> if yet waiting
//     //1-> ongoing
//     //2->vaccinate yourself
//     //3-> successful vaccination
//     int curr_stat;
//     int rounds_already;
//     pthread_mutex_t mutex;

// };

bool get_random_answer(long double prob)
{
    int threshold = (int)((prob)*100);
    int num = get_random_int(0, 100);
    if (num <= threshold)
    {
        return true;
    }
    return false;
}

void seek_hospital(int id)
{
    bool antibody_developed = false;
    while (true)
    {
        //keep on trying unless 3 failures or a success
        printf(ANSI_GREEN "Student id %d\t has arrived for round %d of vaccination\n" ANSI_RESET, id, stu_ptr[id]->rounds_already + 1);
        stu_ptr[id]->curr_stat = 0; //student has started waiting
        printf(ANSI_GREEN "Student id %d\t is waiting to be allocated a slot in a Vaccination Zone\n" ANSI_RESET, id);

        pthread_mutex_lock(&hopeful_mutex);
        hopeful_students_num++;
        pthread_mutex_unlock(&hopeful_mutex);
        while (stu_ptr[id]->curr_stat != 2)
        {
            //pass
        }
        //time to vaccinate
        stu_ptr[id]->rounds_already++;
        long double prob_success = comp_ptr[stu_ptr[id]->vaccine_comp_id]->prob_of_success;
        antibody_developed = get_random_answer(prob_success);
        if (antibody_developed)
        {
            printf(ANSI_CYAN "Student %d has tested positive for antibodies\n" ANSI_RESET, id);
            printf(ANSI_BLUE "Student %d can now attend college\n" ANSI_RESET, id);
            pthread_mutex_lock(&conclusions_mutex);
            tot_conclusions_left--;
            pthread_mutex_unlock(&conclusions_mutex);
            return;
        }
        else
        {
            printf(ANSI_CYAN "Student %d has tested negative for antibodies in his %d th attempt\n" ANSI_RESET, id, stu_ptr[id]->rounds_already);
        }

        if (stu_ptr[id]->rounds_already == 3)
        {
            break;
        }
    }
    pthread_mutex_lock(&conclusions_mutex);
    tot_conclusions_left--;
    pthread_mutex_unlock(&conclusions_mutex);
    printf(ANSI_BLUE "Student %d   is unable to develop anibodies even after 3 attempts,he has to return home\n" ANSI_RESET,id);
    return;

    //update conclusions mutex
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
    // printf(ANSI_GREEN"Student %d\t HAS entered the simulation\n"ANSI_RESET,id);
    seek_hospital(id);

    return NULL;
}