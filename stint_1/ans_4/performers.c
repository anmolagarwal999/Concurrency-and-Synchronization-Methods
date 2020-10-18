#include "master_header.h"
#include "performers.h"

void collect_tshirt(int id)
{
    struct performer *ptr = perf_ptr[id];
    sem_wait(&sem_tshirt_givers);
    printf(ANSI_GREEN "Performer with id %d collecting tshirt\n", id);

    sem_post(&sem_tshirt_givers);
}

void *performer_a_stage(void *ptr2)
{
    int id = *((int *)ptr2);
    struct performer *ptr = perf_ptr[id];
    sleep(ptr->arrival_time);
    bool found_stage = false;
block1:
    sem_wait(&sem_empty_a);
    //unnecessary  but still -> preliminary check-> acquire mutex to make sure that status does not change while reading
    pthread_mutex_lock(&ptr->mutex);

    //preliminary check
    if (ptr->curr_stat != Waiting)
    {
        //No need of this thread any longer
        //Neceassry to unlock as other redundant thread may need it

        //increment semapore for false alarm
        sem_post(&sem_empty_a);
        pthread_mutex_unlock(&ptr->mutex);
        return NULL;
    }
    pthread_mutex_unlock(&ptr->mutex);

    for (int i = 0; i < num_stage_a; i++)
    {
        pthread_mutex_lock(&st_ptr[i]->mutex);
        // enum stage_statuses
        // {
        //     Unoccupied,
        //     one_musician,
        //     one_singer,
        //     two_folks
        // };
        if (st_ptr[i]->curr_stat == Unoccupied)
        {
            //if I am still waiting, i will take this stage
            pthread_mutex_lock(&ptr->mutex);
            if (ptr->curr_stat != Waiting)
            {
                //I have already gotten another stage

                //increment semapore for false alarm
                sem_post(&sem_empty_a);
                pthread_mutex_unlock(&ptr->mutex);

                //Unlock stage mutex which you have locked
                pthread_mutex_unlock(&st_ptr[i]->mutex);

                return NULL;
            }
            else
            {
                //Change self status
                ptr->curr_stat = Performing_solo;

                //Book stage for self
                st_ptr[i]->curr_stat = one_musician;
            }

            pthread_mutex_unlock(&ptr->mutex);
        }
        pthread_mutex_unlock(&st_ptr[i]->mutex);
    }

    if (!found_stage)
    {
        printf("Extremely BAD SHIT HAS HAPPENED -> Moriarty\n");
        exit(0);

        //goto block1->redundant but keep for future debugging
        goto block1;
    }
    else
    {
        give_leader_performance(id);
    }

    return NULL;
}

void give_leader_performance(int id)
{
    struct performer *ptr = perf_ptr[id];
    int perf_type=ptr->type;
    if(perf_type==perf_s)
    {
        //do not invite any singer for duel
    }
    else
    {
        sem_post(&sem_filled_ae);
    }

    //go to sleep
    
}
