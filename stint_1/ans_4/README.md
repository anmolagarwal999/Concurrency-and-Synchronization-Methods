# Q3: Music mayhem


## Running the program

```bash=
make
```

## Notation being followed
> The performers have following types
```c=
enum performer_types
{
    perf_a, //musician who can play acoustic instruments
    perf_e, //musician who can play electric instruments
    perf_ae, //musician who can play both types of instruments
    perf_s //Singers
};
```

 > The performers can have following states:
```c=
enum performer_statuses
{
    Unarrived,
    Waiting,
    Performing_solo,
    open_to_duel,
    Performing_duel,
    Wait_for_shirt,
    Collecting_shirt,
    Left_show
};
```


> The stages can have following statuses:
```
enum stage_statuses
{
    Unoccupied,
    one_occupant,
    two_folks
};
```



## About coordinators
I treat the coordiantors/t-shirt-givers as resources which are being tracked by the `sem_tshirt_givers` semaphore. A person interested in collecting t-shirt can wait on this seamphore and after receiving t-shirt, he/she increments semaphore value as the resource is now freed.


```c=
void collect_tshirt(int id)
{
    sem_wait(&sem_tshirt_givers);

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
```

## About stages
I treat the stages as resources as well. A stage can be in one of the following resource classes at any given time:
* Resource 1: empty_acoustic stage denoted by semaphore `sem_empty_a`
* Resource 2: empty_electrical_stage denoted by semaphore `sem_empty_e`
* Resource 3: stage_occpupied_by_one_musician by semaphore ` sem_filled_ae`
* stage is not a resource currently as it is either doubly occupied or is occupied by a singer


## About Performers and the heart of the logic
Each performer may be open to perform on one or more stages of resource types 1,2 or 3 depending upon performer type. For a particular performer, I created one or more helper threads for this performer, where each helper thread belonging to a particular performer tries to occpuy a stage of a particular type.

```c=
struct performer
{
    //Some data and variables
    pthread_mutex_t mutex;
    pthread_t thread_obj[3];
    int thr_id[3];
    //Some other data and variables

};

```
> ### ***Not each performer uses all of the thread objects allocated to it.***
> If needed, thread_obj indexed at 0 waits for empty_acoustic stage
> If needed, thread_obj indexed at 1 waits for empty_electric stage
> If needed, thread_obj indexed at 2 waits for stages singly occupied by a musician



### **The performers are of 4 types:**

1. **Acoustic musician**
This performer is only open to perform on stages which are resources of type 1.

2. **Electric musician**
This performer is only open to perform on stages which are resources of type 2.
3. **Musician who can play both acoustic and electric instruments**
This performer is only open to perform on stages which are resources of type 1 or 2.
4. **Singer**
This performer is only open to perform on stages which are resources of type 1,2 or 3.

The code which creates and uses threads for a particular type of performer is as follows:

```c=
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
```

## Mechanism of waiting on a stage and trying to occupy it
The helper threads for a particular performer try to get their respective resource types for the performer. They wait on their respective resource semaphores. If they get a chance to occupy a stage, they follow the following procedure (mentioned briefly, see code for specifics):
 1. Acquire performer lock to make sure that other sibling helper threads do not change the status of the performer in between
 2. Check if the performer is still waiting and if this is the case, change performer status and assign the stage to him
 3. If the performer is no loner waiting, this means he is either performing on a stage secured to it by a sibling helper thread  or he has already left after performing on a stage secured for it by another sibling thread. If this is the case, the line of action for current thread is to resignal the semaphore to increment its value as it no longer needs to secure the stage which was meant to be allocated to it.

4. Release performer lock

## Singer-specific action
Only the singer waits on singly_occupied_stages by musicians. However, if the performance of the musician gets over, the singer may no longer get a chance to perform with this particular musician. In this case, the singer may go back to wait on a stage occupied singly by a musician.

## Occupying an empty stage
Musician is occupant | Singer is occupant
------------ | -------------
Signals to resource of type 3 to allow potential duets with waiting singers.Waits for performance to get over. If a singer joins for a duel performance, extends time by 2 sec | Doesn't signal for a potential duel with a fellow singer as requirements mentioned in PDF prohibit 2 singers on stage at the same time

```c=
 if (perf_type == perf_s)
    {
        //do not invite any other singer for duel

        printf(ANSI_GREEN "Singer %s is starting solo on stage id %d for time %d secs\n" ANSI_RESET, ptr->name, stage_id, ptr->perf_time);
    }
    else
    {
        printf(ANSI_RED "Musician %s is starting solo on stage id %d with instrument %c for time %d secs\n" ANSI_RESET, ptr->name, stage_id, ptr->instrument_id, ptr->perf_time);
        pthread_mutex_lock(&st_ptr[stage_id]->mutex);
        st_ptr[stage_id]->curr_stat = open_to_duel;
        pthread_mutex_unlock(&st_ptr[stage_id]->mutex);
        sem_post(&sem_filled_ae);
    }
```
## Other implementation details
* Timed wait on semaphore is used to keep track of when the performer should leave after starvation or lack of resources
* I have used a **dummy rogue semaphore (a semaphore which is never signalled)** and have used the sem_timedwait() to synchronize arrival times of the helper threads belonging to the same performer. This would not have been possible using `sleep`  as sleep works in a relative sense whereas the dummy rogue semaphore works in an absolute sense when used along with the timespec struct.


## Assumptions
* I am assuming that the name has no space-similar characters in between for parsing purposes
* For more rigour, instead of my threads waiting for arrival time from the time they were scheduled, they wait from the time the input was given, to give a more accurate simulation and not to keep waiting time based on a CPU scheduler-dependent decision.
* Number of inputted stages and performers won't excced the timelimit
* Non-negative time inputs are given
* t2>=t1 as input
* 