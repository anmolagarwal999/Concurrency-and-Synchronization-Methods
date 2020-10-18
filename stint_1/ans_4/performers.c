#include "master_header.h"
#include "performers.h"

bool is_musician(int id)
{
    if (perf_ptr[id]->type == perf_s)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void collect_tshirt(int id)
{
    struct performer *ptr = perf_ptr[id];
    sem_wait(&sem_tshirt_givers);

    //redundant status change for doing for consistency
    pthread_mutex_lock(&perf_ptr[id]->mutex);
    perf_ptr[id]->curr_stat = Collecting_shirt;
    pthread_mutex_unlock(&perf_ptr[id]->mutex);
    printf(ANSI_GREEN "Performer with id %d collecting tshirt\n", id);
    sleep(2);
    pthread_mutex_lock(&perf_ptr[id]->mutex);
    perf_ptr[id]->curr_stat = Left_show;
    pthread_mutex_unlock(&perf_ptr[id]->mutex);
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
                ptr->stage_allotted = st_ptr[i]->stage_id;

                //Book stage for self
                st_ptr[i]->curr_stat = one_occupant;
                st_ptr[i]->perf_id1 = ptr->id;
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

void *performer_e_stage(void *ptr2)
{
    int id = *((int *)ptr2);
    struct performer *ptr = perf_ptr[id];
    sleep(ptr->arrival_time);
    bool found_stage = false;
block2:
    sem_wait(&sem_empty_e);
    //unnecessary  but still -> preliminary check-> acquire mutex to make sure that status does not change while reading
    pthread_mutex_lock(&ptr->mutex);

    //preliminary check
    if (ptr->curr_stat != Waiting)
    {
        //No need of this thread any longer
        //Neceassry to unlock as other redundant thread may need it

        //increment semapore for false alarm
        sem_post(&sem_empty_e);
        pthread_mutex_unlock(&ptr->mutex);
        return NULL;
    }
    pthread_mutex_unlock(&ptr->mutex);

    for (int i = num_stage_a; i < tot_num_stages; i++)
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
                sem_post(&sem_empty_e);
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
                st_ptr[i]->curr_stat = one_occupant;
                st_ptr[i]->perf_id1 = ptr->id;
            }

            pthread_mutex_unlock(&ptr->mutex);
        }
        pthread_mutex_unlock(&st_ptr[i]->mutex);
    }

    if (!found_stage)
    {
        printf("Extremely BAD SHIT HAS HAPPENED -> Red John\n");
        exit(0);

        //goto block2->redundant but keep for future debugging
        goto block2;
    }
    else
    {
        give_leader_performance(id);
    }

    return NULL;
}

void *performer_ae_stage(void *ptr3)
{
    int id = *((int *)ptr3);
    struct performer *ptr = perf_ptr[id];
    sleep(ptr->arrival_time);
    bool found_stage = false;
block3:
    sem_wait(&sem_filled_ae);
    //unnecessary  but still -> preliminary check-> acquire mutex to make sure that status does not change while reading
    pthread_mutex_lock(&ptr->mutex);

    //preliminary check
    if (ptr->curr_stat != Waiting)
    {
        //No need of this thread any longer
        //Necessary to unlock as other redundant thread may need it

        //increment semapore for false alarm
        sem_post(&sem_filled_ae);
        pthread_mutex_unlock(&ptr->mutex);
        return NULL;
    }
    pthread_mutex_unlock(&ptr->mutex);

    for (int i = 0; i < tot_num_stages; i++)
    {
        pthread_mutex_lock(&st_ptr[i]->mutex);
        // enum stage_statuses
        // {
        //     Unoccupied,
        //     one_musician,
        //     one_singer,
        //     two_folks
        // };
        if (st_ptr[i]->curr_stat == Performing_solo && is_musician(st_ptr[i]->perf_id1))
        {
            //if I am still waiting, i will take this stage
            pthread_mutex_lock(&ptr->mutex);
            if (ptr->curr_stat != Waiting)
            {
                //I have already gotten another stage

                //increment semapore for false alarm
                sem_post(&sem_filled_ae);
                pthread_mutex_unlock(&ptr->mutex);

                //Unlock stage mutex which you have locked
                pthread_mutex_unlock(&st_ptr[i]->mutex);

                return NULL;
            }
            else
            {
                //Change self status
                ptr->curr_stat = Performing_duel;

                //Book stage for self
                st_ptr[i]->curr_stat = two_folks;
                st_ptr[i]->perf_id2 = ptr->id;
                int leader_id = st_ptr[i]->perf_id1;
                perf_ptr[leader_id]->curr_stat = Performing_duel;
            }

            pthread_mutex_unlock(&ptr->mutex);
        }
        pthread_mutex_unlock(&st_ptr[i]->mutex);
    }

    if (!found_stage)
    {
        printf("False alarm -> Loki\n");
        goto block3;
    }
    else
    {
        // leave matter in leader's hands
    }

    return NULL;
}
void give_leader_performance(int id)
{
    struct performer *ptr = perf_ptr[id];
    int perf_type = ptr->type;
    if (perf_type == perf_s)
    {
        //do not invite any singer for duel
    }
    else
    {
        sem_post(&sem_filled_ae);
    }

    sleep(ptr->perf_time);
    //sleep over
    int leader_id = id;
    int follower_id = -1;
    int stage_id = ptr->stage_allotted;
    if (st_ptr[stage_id]->curr_stat == Unoccupied)
    {
        printf("Weird stuff -> TOKYO\n");
        exit(-1);
    }
    //check if performance was a duel affair
    //wait extra 2 sec if duel

    if (st_ptr[stage_id]->curr_stat == two_folks)
    {
        follower_id = st_ptr[stage_id]->perf_id2;
        sleep(2);
    }

    //change status of one/two performers and display apt messages
    pthread_mutex_lock(&perf_ptr[leader_id]->mutex);
    perf_ptr[leader_id]->curr_stat = Wait_for_shirt;
    pthread_mutex_unlock(&perf_ptr[leader_id]->mutex);

    if (follower_id != -1)
    {
        pthread_mutex_lock(&perf_ptr[follower_id]->mutex);
        perf_ptr[follower_id]->curr_stat = Wait_for_shirt;
        pthread_mutex_unlock(&perf_ptr[follower_id]->mutex);
    }

    //release stages by init status and other variables of stage
    pthread_mutex_lock(&st_ptr[stage_id]->mutex);
    st_ptr[stage_id]->perf_id1 = -1;
    st_ptr[stage_id]->perf_id2 = -1;
    st_ptr[stage_id]->curr_stat = Unoccupied;
    if (st_ptr[id]->type == TYPE_A)
    {
        sem_post(&sem_empty_a);
    }
    else
    {
        sem_post(&sem_empty_e);
    }
    pthread_mutex_unlock(&st_ptr[stage_id]->mutex);

    //send folks for t-shirt collection
    collect_tshirt(leader_id);
    if (follower_id != -1)
    {
        collect_tshirt(follower_id);
    }
}
