#include "master_header.h"
#include "performers.h"

// #define stage_type_a 1
// #define stage_type_e 2

void seek_performer_stage_a(int stage_id)
{

    //wait for one of the performers to come
block1:
    bool found_someone = false;
    sem_wait(&sem_a_ae_s);
    /* EAGAIN
    The operation could not be performed without blocking
     (i.e., the semaphore currently has the value zero).*/

    if (sem_trywait(&sem_a) != EAGAIN)
    {
        //successful
        for (int i = 0; i < nump_a; i++)
        {
            pthread_mutex_lock(&musc_a_ptr[i]->mutex);
            if (musc_a_ptr[i]->curr_stat == Waiting)
            {
                found_someone = true;
                musc_a_ptr[i]->curr_stat = Performing_solo;
                st_ptr[stage_id]->perf_id1 = musc_a_ptr[i]->id;
            }
            pthread_mutex_unlock(&perf_ptr[i]->mutex);
            if (found_someone)
            {
                break;
            }
        }

        if (!found_someone)
        {
            printf(ANSI_RED "WEIRD STUFF IN ALASKA\n" ANSI_RESET);
            exit(-1);
        }
    }
    else
    {
        if (sem_trywait(&sem_ae) != EAGAIN)
        {
            //successful
            for (int i = 0; i < nump_ae; i++)
            {
                pthread_mutex_lock(&musc_ae_ptr[i]->mutex);
                if (musc_ae_ptr[i]->curr_stat == Waiting)
                {
                    found_someone = true;
                    musc_ae_ptr[i]->curr_stat = Performing_solo;
                    st_ptr[stage_id]->perf_id1 = musc_ae_ptr[i]->id;
                }
                pthread_mutex_unlock(&perf_ptr[i]->mutex);
                if (found_someone)
                {
                    break;
                }
            }

            if (!found_someone)
            {
                printf(ANSI_RED "WEIRD STUFF IN Moscow\n" ANSI_RESET);
                exit(-1);
            }
        }
        else
        {
            if (sem_trywait(&sem_s) != EAGAIN)
            {
                //successful
                for (int i = 0; i < nump_s; i++)
                {
                    pthread_mutex_lock(&singer_ptr[i]->mutex);
                    if (singer_ptr[i]->curr_stat == Waiting)
                    {
                        found_someone = true;
                        singer_ptr[i]->curr_stat = Performing_solo;
                        st_ptr[stage_id]->perf_id1 = singer_ptr[i]->id;
                    }
                    pthread_mutex_unlock(&perf_ptr[i]->mutex);
                    if (found_someone)
                    {
                        break;
                    }
                }

                if (!found_someone)
                {
                    printf(ANSI_RED "WEIRD STUFF IN Miami\n" ANSI_RESET);
                    exit(-1);
                }
            }
            else
            {
                printf(ANSI_RED "No one was convinced to come to this stage with id %d\n" ANSI_RESET,stage_id);
                part2;
                part2;
            }
        }
    }

    if (!found_someone)
    {
        goto block1;
    }
    else
    {
        //let the found guy perform
    }
}


void seek_performer_stage_a(int stage_id)
{

    //wait for one of the performers to come
block1:
    bool found_someone = false;
    sem_wait(&sem_a_ae_s);
    /* EAGAIN
    The operation could not be performed without blocking
     (i.e., the semaphore currently has the value zero).*/

    if (sem_trywait(&sem_a) != EAGAIN)
    {
        //successful
        for (int i = 0; i < nump_a; i++)
        {
            pthread_mutex_lock(&musc_a_ptr[i]->mutex);
            if (musc_a_ptr[i]->curr_stat == Waiting)
            {
                found_someone = true;
                musc_a_ptr[i]->curr_stat = Performing_solo;
                st_ptr[stage_id]->perf_id1 = musc_a_ptr[i]->id;
            }
            pthread_mutex_unlock(&perf_ptr[i]->mutex);
            if (found_someone)
            {
                break;
            }
        }

        if (!found_someone)
        {
            printf(ANSI_RED "WEIRD STUFF IN ALASKA\n" ANSI_RESET);
            exit(-1);
        }
    }
    else
    {
        if (sem_trywait(&sem_ae) != EAGAIN)
        {
            //successful
            for (int i = 0; i < nump_ae; i++)
            {
                pthread_mutex_lock(&musc_ae_ptr[i]->mutex);
                if (musc_ae_ptr[i]->curr_stat == Waiting)
                {
                    found_someone = true;
                    musc_ae_ptr[i]->curr_stat = Performing_solo;
                    st_ptr[stage_id]->perf_id1 = musc_ae_ptr[i]->id;
                }
                pthread_mutex_unlock(&perf_ptr[i]->mutex);
                if (found_someone)
                {
                    break;
                }
            }

            if (!found_someone)
            {
                printf(ANSI_RED "WEIRD STUFF IN Moscow\n" ANSI_RESET);
                exit(-1);
            }
        }
        else
        {
            if (sem_trywait(&sem_s) != EAGAIN)
            {
                //successful
                for (int i = 0; i < nump_s; i++)
                {
                    pthread_mutex_lock(&singer_ptr[i]->mutex);
                    if (singer_ptr[i]->curr_stat == Waiting)
                    {
                        found_someone = true;
                        singer_ptr[i]->curr_stat = Performing_solo;
                        st_ptr[stage_id]->perf_id1 = singer_ptr[i]->id;
                    }
                    pthread_mutex_unlock(&perf_ptr[i]->mutex);
                    if (found_someone)
                    {
                        break;
                    }
                }

                if (!found_someone)
                {
                    printf(ANSI_RED "WEIRD STUFF IN Miami\n" ANSI_RESET);
                    exit(-1);
                }
            }
            else
            {
                printf(ANSI_RED "No one was convinced to come to this stage with id %d\n" ANSI_RESET,stage_id);
                part2;
                part2;
            }
        }
    }

    if (!found_someone)
    {
        goto block1;
    }
    else
    {
        //let the found guy perform
    }
}

void seek_performer_stage_one_occupant()
{

block3:
    bool found_someone = false;
    ////////////////////////
    // sem_timedwait(&sem_s);
    ///////////////////////////
}

void *stage_entry(void *ptr)
{
    int id = *((int *)ptr);

    return NULL;
}