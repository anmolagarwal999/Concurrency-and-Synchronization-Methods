void a_musician()
{
ap:
    sem_wait(sem_a);
    {

        ret = sem_try_wait(tot);
        if (ret = SUCCESS)
        {
            assign_STAGE
        }
        else
        {
            goto ap;
        }
    }
}

void(ae) musician()
{
    sem_wait(sem_tot);
    ret2 = sem_try_wait(sem_a);
    if (ret = SUCCESS)
    {
        assign_STAGE
    }
    else
    {
        ret3 = sem_try_wait(e);
    }
}

//for all-1 -> free-acqu+free--electr
//for ac-2 -. free acq
//for ele-3  free ele
//for singer-4->    free-acqu + free--electr+ 1musician on stages

void acquostic()
{

    //done with previous performance

    signal_sem(sem2);
    signal_sem(sem1);
    signal_sem(sem4)
}

void electrcic()
{
    signal_sem(sem3);
    signal_sem(sem1);
    signal_sem(sem4);
}