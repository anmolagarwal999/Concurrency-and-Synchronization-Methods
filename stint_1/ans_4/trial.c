void seek_performer_stage_a()
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
            pthread_mutex_lock(&perf_ptr[i]->mutex);
            if (perf_ptr[i]->curr_stat == Waiting)
            {
                //let this guy perform
                //change his status, waiting for his performance
            }
            pthread_mutex_unlock(&perf_ptr[i]->mutex);
        }
    }
    else
    {
        if (sem_trywait(&sem_ae) != EAGAIN)
        {
            //successful
            for (int i = 0; i < nump_ae; i++)
            {
                pthread_mutex_lock(&perf_ptr[i]->mutex);
                if (perf_ptr[i]->curr_stat == Waiting)
                {
                    //let this guy perform
                    //change his status, waiting for his performance
                }
                pthread_mutex_unlock(&perf_ptr[i]->mutex);
            }
        }
        else
        {
            if (sem_trywait(&sem_s) != EAGAIN)
            {
                //successful
                for (int i = 0; i < nump_s; i++)
                {
                    pthread_mutex_lock(&perf_ptr[i]->mutex);
                    if (perf_ptr[i]->curr_stat == Waiting)
                    {
                        //let this guy perform
                        //change his status, waiting for his performance
                    }
                    pthread_mutex_unlock(&perf_ptr[i]->mutex);
                }
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