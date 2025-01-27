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
    sem_wait(&sem_tshirt_givers);

    //redundant status change but still doing for consistency
    pthread_mutex_lock(&perf_ptr[id]->mutex);
    perf_ptr[id]->curr_stat = Collecting_shirt;
    pthread_mutex_unlock(&perf_ptr[id]->mutex);
    printf(BMAG "Performer with name %s collecting tshirt\n" ANSI_RESET, perf_ptr[id]->name);
    sleep(2);
    pthread_mutex_lock(&perf_ptr[id]->mutex);
    perf_ptr[id]->curr_stat = Left_show;
    pthread_mutex_unlock(&perf_ptr[id]->mutex);
    sem_post(&sem_tshirt_givers);
    printf(BCYN "Performer %s collected TSHIRT and left the show\n" ANSI_RESET, perf_ptr[id]->name);
}

void *performer_empty_a_stage(void *ptr1)
{
    int id = *((int *)ptr1);
    struct performer *ptr = perf_ptr[id];
    // printf(ANSI_RED "rogue id is %d\n" ANSI_RESET, id);
    sem_timedwait(&rogue_sem, perf_ptr[id]->st_arrival);
    pthread_mutex_lock(&ptr->mutex);
    if (ptr->curr_stat == Unarrived)
    {
        ptr->curr_stat = Waiting;
        printf(BYEL "Performer %s has arrived and plays instrument %c\n" ANSI_RESET, perf_ptr[id]->name, perf_ptr[id]->instrument_id);
    }
    pthread_mutex_unlock(&ptr->mutex);

    int ret1;
    bool found_stage = false;
    struct timespec *st = perf_ptr[id]->st_leave;

    //printf("Entered searching for acqoustic stage\n");
block1:
    found_stage = false;
    ret1 = sem_timedwait(&sem_empty_a, st);
    if (ret1 == 0)
    {
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

        for (int i = 0; i < num_stage_a && found_stage == false; i++)
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
                    found_stage = true;
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
            // exit(0);

            //goto block1->redundant but keep for future debugging
            goto block1;
        }
        else
        {
            give_leader_performance(id);
        }
    }
    else
    {
        //display that he left as he was impatient
        pthread_mutex_lock(&ptr->mutex);
        //still check if still waiting, change to leftover and display message
        if (ptr->curr_stat == Waiting)
        {
            ptr->curr_stat = Left_show;
            printf(BCYN "Performer %s became impatient and left the show\n" ANSI_RESET, perf_ptr[id]->name);
        }
        //if not, then either some other thread got a stage for performer or the other thread already left
        //nothing to be done, just leave
        pthread_mutex_unlock(&ptr->mutex);
    }

    return NULL;
}

void *performer_empty_e_stage(void *ptr2)
{
    int id = *((int *)ptr2);
    struct performer *ptr = perf_ptr[id];
    //printf(ANSI_RED "rogue id is %d\n" ANSI_RESET, id);

    bool found_stage = false;
    struct timespec *st = perf_ptr[id]->st_leave;
    sem_timedwait(&rogue_sem, perf_ptr[id]->st_arrival);

    pthread_mutex_lock(&ptr->mutex);
    if (ptr->curr_stat == Unarrived)
    {
        ptr->curr_stat = Waiting;
        printf(BYEL "Performer %s has arrived and plays instrument %c\n" ANSI_RESET, perf_ptr[id]->name, perf_ptr[id]->instrument_id);
    }
    pthread_mutex_unlock(&ptr->mutex);

    // printf("Entered searching for elctric stage\n");
    int ret2;

block2:
    found_stage = false;
    ret2 = sem_timedwait(&sem_empty_e, st);

    if (ret2 == 0)
    {
        //unnecessary  but still -> preliminary check-> acquire mutex to make sure that status does not change while reading
        pthread_mutex_lock(&ptr->mutex);

        //preliminary check
        if (ptr->curr_stat != Waiting)
        {
            //No need of this thread any longer
            //Neceassry to unlock as other redundant thread may need it

            //increment semapore for false alarm
            // printf(ANSI_RED "FALSE ALARM . I AM PRIVILEGED status is %d\n" ANSI_RESET, ptr->curr_stat);
            sem_post(&sem_empty_e);
            pthread_mutex_unlock(&ptr->mutex);
            return NULL;
        }
        pthread_mutex_unlock(&ptr->mutex);

        for (int i = num_stage_a; i < tot_num_stages && found_stage == false; i++)
        {
            //debug(i);
            // printf("investigating stage with id %d\n", i);
            pthread_mutex_lock(&st_ptr[i]->mutex);
            // printf("Able to get mutex of stage with id %d| Status is %d\n", st_ptr[i]->stage_id, st_ptr[i]->curr_stat);
            fflush(stdout);
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
                //printf("Found stage unoccupied\n");
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
                    found_stage = true;
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
            printf("Extremely BAD SHIT HAS HAPPENED -> Red John\n");
            //exit(0);

            //goto block2->redundant but keep for future debugging
            goto block2;
        }
        else
        {
            // printf("I have been able to find a stage\n");
            give_leader_performance(id);
        }
    }
    else
    {
        //display that he left as he was impatient
        pthread_mutex_lock(&ptr->mutex);
        //still check if still waiting, change to leftover and display message
        if (ptr->curr_stat == Waiting)
        {
            ptr->curr_stat = Left_show;
            printf(BCYN "Performer %s became impatient and left the show\n" ANSI_RESET, perf_ptr[id]->name);
        }
        //if not, then either some other thread got a stage for performer or the other thread already left
        //nothing to be done, just leave
        pthread_mutex_unlock(&ptr->mutex);
    }

    return NULL;
}

void *performer_filled_ae_stage(void *ptr3)
{
    int id = *((int *)ptr3);
    struct performer *ptr = perf_ptr[id];
    sem_timedwait(&rogue_sem, perf_ptr[id]->st_arrival);

    pthread_mutex_lock(&ptr->mutex);
    if (ptr->curr_stat == Unarrived)
    {
        ptr->curr_stat = Waiting;
        printf(BYEL "Performer %s has arrived and plays instrument %c\n" ANSI_RESET, perf_ptr[id]->name, perf_ptr[id]->instrument_id);
    }
    pthread_mutex_unlock(&ptr->mutex);
    struct timespec *st = perf_ptr[id]->st_leave;

    bool found_stage = false;
    int ret3;
block3:
    found_stage = false;
    ret3 = sem_timedwait(&sem_filled_ae, st);
    //unnecessary  but still -> preliminary check-> acquire mutex to make sure that status does not change while reading
    if (ret3 == 0)
    {
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

        for (int i = 0; i < tot_num_stages && found_stage == false; i++)
        {
            //printf("trying to acquire lock for DUEL FUNC %d\n", i);
            pthread_mutex_lock(&st_ptr[i]->mutex);
            // enum stage_statuses
            // {
            //     Unoccupied,
            //     one_musician,
            //     one_singer,
            //     two_folks
            // };
            if (st_ptr[i]->curr_stat == open_to_duel && is_musician(st_ptr[i]->perf_id1))
            {
                //if I am still waiting, i will take this stage
                pthread_mutex_lock(&ptr->mutex);
                if (ptr->curr_stat != Waiting)
                {
                    //I have already gotten another stage

                    //increment semaphore for false alarm
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
                    found_stage = true;

                    //Book stage for self
                    st_ptr[i]->curr_stat = two_folks;
                    st_ptr[i]->perf_id2 = ptr->id;
                    ptr->stage_allotted = st_ptr[i]->stage_id;

                    int leader_id = st_ptr[i]->perf_id1;
                    perf_ptr[leader_id]->curr_stat = Performing_duel;
                    printf(ANSI_BLUE "Singer %s is starting DUEL with %s on stage id %d %s\n" ANSI_RESET, ptr->name,perf_ptr[leader_id]->name, st_ptr[i]->stage_id,st_print_ptr->name[st_ptr[i]->type] );
                }

                pthread_mutex_unlock(&ptr->mutex);
            }
            pthread_mutex_unlock(&st_ptr[i]->mutex);
        }

        if (!found_stage)
        {
            // printf("False alarm -> Loki\n");
            goto block3;
        }
        else
        {
            // leave matter in leader's hands
        }
    }
    else
    {
        //display that he left as he was impatient
        pthread_mutex_lock(&ptr->mutex);
        //still check if still waiting, change to leftover and display message
        if (ptr->curr_stat == Waiting)
        {
            ptr->curr_stat = Left_show;
            printf(BCYN "Performer %s became impatient and left the show\n" ANSI_RESET, perf_ptr[id]->name);
        }
        //if not, then either some other thread got a stage for performer or the other thread already left
        //nothing to be done, just leave
        pthread_mutex_unlock(&ptr->mutex);
    }

    return NULL;
}

void give_leader_performance(int id)
{
    struct performer *ptr = perf_ptr[id];
    int perf_type = ptr->type;
    int stage_id = ptr->stage_allotted;
    if (perf_type == perf_s)
    {
        //do not invite any other singer for duel

        //printf("SOLO SINGER TIME IS %d\n", perf_ptr[id]->perf_time);
        printf(ANSI_GREEN "Singer %s is starting solo on stage id %d %s for time %d secs\n" ANSI_RESET, ptr->name, stage_id, st_print_ptr->name[st_ptr[stage_id]->type], ptr->perf_time);
    }
    else
    {
        printf(ANSI_RED "Musician %s is starting solo on stage id %d %s with instrument %c for time %d secs\n" ANSI_RESET, ptr->name, stage_id, st_print_ptr->name[st_ptr[stage_id]->type], ptr->instrument_id, ptr->perf_time);
        pthread_mutex_lock(&st_ptr[stage_id]->mutex);
        st_ptr[stage_id]->curr_stat = open_to_duel;
        pthread_mutex_unlock(&st_ptr[stage_id]->mutex);
        sem_post(&sem_filled_ae);
    }

    sleep(ptr->perf_time);
    //sleep over
    int leader_id = id;
    int follower_id = -1;
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
    printf(BGRN "Performer %s has ended his/her performance on stage id %d %s\n" ANSI_RESET, perf_ptr[leader_id]->name, stage_id, st_print_ptr->name[st_ptr[stage_id]->type]);
    perf_ptr[leader_id]->curr_stat = Wait_for_shirt;
    pthread_mutex_unlock(&perf_ptr[leader_id]->mutex);

    if (follower_id != -1)
    {
        pthread_mutex_lock(&perf_ptr[follower_id]->mutex);
        printf(BGRN "Performer %s has ended his/her DUEL performance on stage id %d %s\n" ANSI_RESET, perf_ptr[follower_id]->name, stage_id, st_print_ptr->name[st_ptr[stage_id]->type]);

        perf_ptr[follower_id]->curr_stat = Wait_for_shirt;
        pthread_mutex_unlock(&perf_ptr[follower_id]->mutex);
    }

    //release stages by init status and other variables of stage
    pthread_mutex_lock(&st_ptr[stage_id]->mutex);
    st_ptr[stage_id]->perf_id1 = -1;
    st_ptr[stage_id]->perf_id2 = -1;
    st_ptr[stage_id]->curr_stat = Unoccupied;
    if (st_ptr[stage_id]->type == TYPE_A)
    {
        // printf("incrementing sempahore of type A\n");
        sem_post(&sem_empty_a);
    }
    else
    {
        // printf("incrementing sempahore of type E\n");

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

void dispatch_performers(int id)
{
    int performer_type = perf_ptr[id]->type;
    // part3;
    // printf("GOING TO CREATE THREADS FOR id %d\n", id);
    // part3;
    perf_ptr[id]->st_arrival = get_abs_time_obj(perf_ptr[id]->arrival_time);
    perf_ptr[id]->st_leave = get_abs_time_obj(perf_ptr[id]->arrival_time + patience_time);
    if (performer_type == perf_a)
    {
        perf_ptr[id]->thr_id[0] = pthread_create(&(perf_ptr[id]->thread_obj[0]), NULL, performer_empty_a_stage, (void *)(&(perf_ptr[id]->id)));
    }
    else if (performer_type == perf_e)
    {
        perf_ptr[id]->thr_id[1] = pthread_create(&(perf_ptr[id]->thread_obj[1]), NULL, performer_empty_e_stage, (void *)(&(perf_ptr[id]->id)));
    }
    else if (performer_type == perf_ae)
    {
        perf_ptr[id]->thr_id[0] = pthread_create(&(perf_ptr[id]->thread_obj[0]), NULL, performer_empty_a_stage, (void *)(&(perf_ptr[id]->id)));
        perf_ptr[id]->thr_id[1] = pthread_create(&(perf_ptr[id]->thread_obj[1]), NULL, performer_empty_e_stage, (void *)(&(perf_ptr[id]->id)));
    }
    else if (performer_type == perf_s)
    {
        perf_ptr[id]->thr_id[0] = pthread_create(&(perf_ptr[id]->thread_obj[0]), NULL, performer_empty_a_stage, (void *)(&(perf_ptr[id]->id)));
        perf_ptr[id]->thr_id[1] = pthread_create(&(perf_ptr[id]->thread_obj[1]), NULL, performer_empty_e_stage, (void *)(&(perf_ptr[id]->id)));
        perf_ptr[id]->thr_id[2] = pthread_create(&(perf_ptr[id]->thread_obj[2]), NULL, performer_filled_ae_stage, (void *)(&(perf_ptr[id]->id)));
    }
}
