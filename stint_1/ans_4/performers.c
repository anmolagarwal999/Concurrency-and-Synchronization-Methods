#include "master_header.h"
#include "performers.h"

void collect_tshirt(int id)
{
    struct performer *ptr = perf_ptr[id];
    sem_wait(&sem_tshirt_givers);
    printf(ANSI_GREEN "Performer with id %d collecting tshirt\n", id);

    sem_post(&sem_tshirt_givers);
}
void seek_stage(int id)
{
    //signal respective semaphores and then go to sleep
    struct performer *ptr = perf_ptr[id];
    int perf_type = ptr->type;

    //https://stackoverflow.com/questions/40153968/does-a-thread-own-a-mutex-after-pthread-cond-timedwait-times-out?rq=1
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //acquire lock so that no one can signal you before you go to sleep
    pthread_mutex_lock(&ptr->mutex);
    //change status to waiting
    ptr->curr_stat = Waiting;
    pthread_mutex_unlock(&ptr->mutex);

    //signal respective semaphores
    if (perf_type == perf_a)
    {
        printf("PERFORMER TYPE IS ACQ\n");
        sem_post(&sem_a);
        sem_post(&sem_a_ae_s);
    }
    else if (perf_type == perf_e)
    {
        printf("PERFORMER TYPE IS ELECTRIC\n");

        sem_post(&sem_e);
        sem_post(&sem_a_ae_s);
    }
    else if (perf_type == perf_ae)
    {
        printf("PERFORMER TYPE IS AE BOTH\n");

        sem_post(&sem_ae);
        sem_post(&sem_a_ae_s);
        sem_post(&sem_e_ae_s);
    }
    else if (perf_type == perf_s)
    {
        printf("PERFORMER TYPE IS SINGER\n");

        sem_post(&sem_s);
        sem_post(&sem_a_ae_s);
        sem_post(&sem_e_ae_s);
    }
    else
    {
        printf(ANSI_RED "Weird stuff in Copenganhagen\n" ANSI_RESET);
        exit(0);
    }

    ///////////////////////////////////////////////////
    //Set waiting time limit
    pthread_mutex_lock(&ptr->mutex);
    pthread_mutex_lock(&ptr->mutex2);

    struct timespec *st = get_abs_time_obj(patience_time);
    //https://man7.org/linux/man-pages/man3/pthread_cond_timedwait.3p.html
    int ret_val = pthread_cond_timedwait(&(ptr->cv), &(ptr->mutex), st);
    printf("ret value is %d\n", ret_val);

    //MUTEX IS WITH ME CURRENTLY
    if (ret_val == ETIMEDOUT)
    {
        printf(ANSI_YELLOW "TIMER EXPIRED for id %d\n" ANSI_RESET, id);
        int stage_got = perf_ptr[id]->stage_allotted;
        if (stage_got == -1)
        {
            //didn't get a stage
            printf(ANSI_GREEN "Performer %d could not get a stage\n" ANSI_RESET, id);
            perf_ptr[id]->curr_stat = Left_show;
            pthread_mutex_unlock(&ptr->mutex);
        }
        else
        {
            //got a stage
            goto got_stage;
        }
    }
    else
    {
    got_stage:
        pthread_mutex_unlock(&ptr->mutex);

        //surely got a stage, wait for performance to get over
        printf(ANSI_GREEN "Performer %d GOT STAGE\n" ANSI_RESET, id);

        pthread_cond_wait(&(ptr->cv), &(ptr->mutex2));
        printf(ANSI_GREEN "Performer %d needs a t-shirt\n" ANSI_RESET, id);
        collect_tshirt(id);
        pthread_mutex_unlock(&ptr->mutex2);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //set performance time and hope for the best

    ////////////////////////////////////////////////////////////////////////////////////
    //if performance done, collect t-shirt
}

void *performer_entry(void *ptr)
{
    int id = *((int *)ptr);

    //wait for arrival time and then start
    debug(id);
    printf(ANSI_BLUE "Proposed arrival time is %d\n" ANSI_RESET, perf_ptr[id]->arrival_time);
    sleep(perf_ptr[id]->arrival_time);
    perf_ptr[id]->perf_time = get_random_int2(t1, t2, "Pegasus");
    seek_stage(id);
    return NULL;
}