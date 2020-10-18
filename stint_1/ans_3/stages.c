#include "master_header.h"
#include "performers.h"

// #define stage_type_a 1
// #define stage_type_e 2

void seek_performer(int stage_id)
{

    //wait for one of the performers to come
    debug(stage_id);
    st_ptr[stage_id]->perf_id1 = -1;
    st_ptr[stage_id]->perf_id2 = -1;
    bool found_someone;
    int curr_st_type = st_ptr[stage_id]->type;
block1:
    st_ptr[stage_id]->perf_id1 = -1;
    st_ptr[stage_id]->perf_id2 = -1;
    found_someone = false;
    printf(ANSI_YELLOW "Stage with id %d is going to wait on semaphore\n" ANSI_RESET, stage_id);
    if (curr_st_type == stage_type_a)
    {
        sem_wait(&sem_a_ae_s);
        printf("Stage %d Succesfully decremented ENTRY A SEMAPHORE\n", stage_id);

        /* EAGAIN
    The operation could not be performed without blocking
     (i.e., the semaphore currently has the value zero).*/

        if (sem_trywait(&sem_a) == 0)
        {
            //successful
            printf("Stage %d Succesfully decremented semaphore of ACQUOSTUC\n", stage_id);

            for (int i = 0; i < nump_a; i++)
            {
                pthread_mutex_lock(&musc_a_ptr[i]->mutex);
                if (musc_a_ptr[i]->curr_stat == Waiting)
                {
                    found_someone = true;
                    musc_a_ptr[i]->curr_stat = Performing_solo;
                    st_ptr[stage_id]->perf_id1 = musc_a_ptr[i]->id;
                    musc_a_ptr[i]->stage_allotted = stage_id;
                    pthread_cond_signal(&musc_a_ptr[i]->cv);
                }
                pthread_mutex_unlock(&musc_a_ptr[i]->mutex);
                if (found_someone)
                {
                    break;
                }
                printf("Stage A %d rejected by %d\n", stage_id, i);
            }

            if (!found_someone)
            {
                printf(ANSI_RED "WEIRD STUFF IN ALASKA\n" ANSI_RESET);
                // exit(-1);
            }
            else
            {
                goto endblock;
            }
        }
    }
    else
    {
        sem_wait(&sem_e_ae_s);
        printf("Stage %d Succesfully decremented ENTRY E SEMAPHORE\n", stage_id);

        /* EAGAIN
    The operation could not be performed without blocking
     (i.e., the semaphore currently has the value zero).*/

        if (sem_trywait(&sem_e) == 0)
        {
            //successful
            printf("Stage %d Succesfully decremented semaphore of ELETCRICr\n", stage_id);

            for (int i = 0; i < nump_e; i++)
            {
                pthread_mutex_lock(&musc_e_ptr[i]->mutex);
                if (musc_e_ptr[i]->curr_stat == Waiting)
                {
                    found_someone = true;
                    musc_e_ptr[i]->curr_stat = Performing_solo;
                    st_ptr[stage_id]->perf_id1 = musc_e_ptr[i]->id;
                    musc_e_ptr[i]->stage_allotted = stage_id;
                    pthread_cond_signal(&musc_e_ptr[i]->cv);
                }
                pthread_mutex_unlock(&musc_e_ptr[i]->mutex);
                if (found_someone)
                {
                    break;
                }
                printf("Stage E %d rejected by %d\n", stage_id, i);
            }

            if (!found_someone)
            {
                printf(ANSI_RED "WEIRD STUFF IN Montreal\n" ANSI_RESET);
                // exit(-1);
            }
            else
            {
                goto endblock;
            }
        }
    }
    /////////////////////////////////////////////  ////////////////////////////////////////////////////////
    int check_ae_val;
    sem_getvalue(&sem_ae, &check_ae_val);
    printf(ANSI_RED "Stage id %d found sem_ae value as %d\n" ANSI_RESET, stage_id, check_ae_val);
    if (sem_trywait(&sem_ae) == 0)
    {
        printf("Stage %d Succesfully decremented semaphore of AE FOLKS\n", stage_id);
        fflush(stdout);
        //successful
        for (int i = 0; i < nump_ae; i++)
        {
            printf(ANSI_MAGENTA "Stage id : %d -> i-> %d in AE stage\n " ANSI_RESET, stage_id, i);
            fflush(stdout);
            pthread_mutex_lock(&musc_ae_ptr[i]->mutex);
            if (musc_ae_ptr[i]->curr_stat == Waiting)
            {
                found_someone = true;
                musc_ae_ptr[i]->curr_stat = Performing_solo;
                st_ptr[stage_id]->perf_id1 = musc_ae_ptr[i]->id;
                musc_ae_ptr[i]->stage_allotted = stage_id;
                pthread_cond_signal(&musc_ae_ptr[i]->cv);
            }
            else
            {
                if (musc_ae_ptr[i]->curr_stat != Waiting)
                {
                    printf("Status of AE %d is not waiting\n", i);
                }
            }

            pthread_mutex_unlock(&musc_ae_ptr[i]->mutex);
            if (found_someone)
            {
                break;
            }
            printf("Stage  %d rejected by AE person %d\n", stage_id, i);
        }

        if (!found_someone)
        {
            printf(ANSI_RED "WEIRD STUFF IN Moscow\n" ANSI_RESET);
            //exit(-1);
        }
        else
        {
            goto endblock;
        }
    }
    else
    {
        if (sem_trywait(&sem_s) == 0)
        {
            //successful
            printf("Stage %d Succesfully decremented semaphore of singer\n", stage_id);
            for (int i = 0; i < nump_s; i++)
            {
                pthread_mutex_lock(&singer_ptr[i]->mutex);
                if (singer_ptr[i]->curr_stat == Waiting)
                {
                    found_someone = true;
                    singer_ptr[i]->curr_stat = Performing_solo;
                    st_ptr[stage_id]->perf_id1 = singer_ptr[i]->id;
                    singer_ptr[i]->stage_allotted = stage_id;
                    pthread_cond_signal(&singer_ptr[i]->cv);
                }
                pthread_mutex_unlock(&singer_ptr[i]->mutex);
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
            else
            {
                        goto endblock;

            }
            
        }
        else
        {
            printf(ANSI_RED "No one was convinced to come to this stage with id %d\n" ANSI_RESET, stage_id);
            part2;
            part2;
        }
    }

endblock:
    if (!found_someone)
    {
        goto block1;
    }
    else
    {
        //let the found guy perform

        seek_other_occupant_if_legal(stage_id);

        int leader_id = st_ptr[stage_id]->perf_id1;
        int follower_id = st_ptr[stage_id]->perf_id2;
        pthread_mutex_lock(&perf_ptr[leader_id]->mutex2);
        pthread_cond_signal(&perf_ptr[leader_id]->cv);
        pthread_mutex_unlock(&perf_ptr[leader_id]->mutex2);
        if (follower_id != -1)
        {
            pthread_mutex_lock(&perf_ptr[follower_id]->mutex2);
            pthread_cond_signal(&perf_ptr[follower_id]->cv);
            pthread_mutex_unlock(&perf_ptr[follower_id]->mutex2);
        }
        goto block1;
    }
}

void seek_other_occupant_if_legal(int stage_id)
{
    int already_id = st_ptr[stage_id]->perf_id1;
    printf(ANSI_CYAN "SEARCHING FOR FOLLOWER for leader %d in sage id %d\n" ANSI_RESET, already_id, stage_id);
    struct performer *leader = perf_ptr[already_id];
    bool found_someone;
    printf("Performance time is %d\n", leader->perf_time);
    struct timespec *st = get_abs_time_obj(leader->perf_time);
    if (leader->type == perf_s)
    {
        //just wait
        //already happening at bottom
        //Don't meddle with this part
    }
    else
    {

    block3:
        found_someone = false;
        ////////////////////////
        // sem_timedwait(&sem_s);
        ///////////////////////////
        int ret_val = sem_timedwait(&sem_s, st);
        if (ret_val != 0)
        {
            printf("TImer ran out and could't find a singer\n");
        }
        else
        {
            for (int i = 0; i < nump_s; i++)
            {
                pthread_mutex_lock(&singer_ptr[i]->mutex);
                if (singer_ptr[i]->curr_stat == Waiting)
                {
                    found_someone = true;
                    singer_ptr[i]->curr_stat = Performing_duel;
                    st_ptr[stage_id]->perf_id2 = singer_ptr[i]->id;
                    printf("SINGER HAS JOINED MUSICIAN ON THE STAGE\n");
                    singer_ptr[i]->stage_allotted = stage_id;
                    pthread_cond_signal(&singer_ptr[i]->cv);
                }
                pthread_mutex_unlock(&singer_ptr[i]->mutex);
                if (found_someone)
                {
                    break;
                }
            }

            if (!found_someone)
            {
                printf(ANSI_RED "WEIRD STUFF IN Berlin\n" ANSI_RESET);
                //exit(-1);
            }
            else
            {
                st->tv_sec += 2;
            }
        }
    }
    sem_timedwait(&rogue_sem, st);
}

void *stage_entry(void *ptr)
{
    int id = *((int *)ptr);
    printf("Going to call seekk_performer\n");
    seek_performer(id);

    return NULL;
}