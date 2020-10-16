
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>
sem_t stage_tracker;

void stage_is_now_free()
{

    ///////////////////??
    //FOR acqustic stage
    ///////////////////////
code:
    int stat1 = try_sem_wait(sem_acquostic_musicians);
    if (stat1 == successful)
    {
        //iterate th
        allow(acqoustic_musician);
        goto work_done;
    }

    int stat2 = try_sem_wait(sem_both_musicians);
    if (stat2 == successful)
    {
        allow(both_musician);
        goto work_done;
    }
    int stat3 = try_sem_wait(singers);
    if (stat3 == successful)
    {
        allow(singers);
        work_done;
    }

    //no suitable performer
    cond_wait(acq_stage_wait);
    goto code;

work_done:
    //code
}

void performer_wants_to_find_stage()
{

    //////////
    //if singer
    //////////////

    signal(singer_sem);
    //change status of singer to available
    cond_singal(acquostic_stages);
    cond_signal(electric_stages);
    cond_signal(stage_already_with_musician);
    



    cond_timed_wait(personal_c[id]);
}