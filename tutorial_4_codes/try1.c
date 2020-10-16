
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>
sem_t stage_tracker;

int num_stages = 100;

bool stage_is_available[100];

void find_stage()
{

part_1:

    sem_wait(&stage_tracker);
    //loop through all stages
    bool got_appropriate_stage = false;
    for (int i = 0; i < num_stages; i++)
    {
        lock(stage_mutex[i]);
        if (stage_is_available[i])
        {
            if (stage_is_appropriate_for_performer)
            {
                got_appropriate_stage = true;
                change_stage_status();
            }
        }
        unlock(stage_mutex[i]);
        if (got_appropriate_stage)
        {
            break;
        }
    }

    if (got_appropriate_stage)
    {
        //
    }
    else
    {
        //false alarm
        sem_post(&stage_tracker);
        goto part_1;
    }
}