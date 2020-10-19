#include "master_header.h"

int tot_num_performers;
int nump_a = 0, nump_e = 0, nump_ae = 0, nump_s = 0;
int t1, t2, patience_time;
int num_stage_a, num_stage_e;
int tot_num_stages;
int num_coordinators;
sem_t sem_empty_a, sem_empty_e, sem_filled_ae;
sem_t sem_coordinators, sem_tshirt_givers, rogue_sem;

int get_performer_type(char ch)
{
    // part;
    // printf("char input is %c\n", ch);
    // part;
    switch (ch)
    {
    case 'p':
        return perf_ae;
    case 'g':
        return perf_ae;
    case 'v':
        return perf_a;
    case 'b':
        return perf_e;
    case 's':
        return perf_s;
    default:
        return perf_ae;
    }
}

void take_input()
{
    printf("Enter number of performers:");
    scanf("%d", &tot_num_performers);
    fflush(stdout);

   // printf(ANSI_RED "REACHED HERE" ANSI_RESET);

    printf("Enter number of acoustic stages: ");
    scanf("%d", &num_stage_a);
    fflush(stdout);

    printf("Enter number of electric stages: ");
    scanf("%d", &num_stage_e);
    fflush(stdout);

    printf("Enter number of coordinators: ");
    scanf("%d", &num_coordinators);
    fflush(stdout);

    printf("Enter min_performance_time: ");
    scanf("%d", &t1);
    fflush(stdout);

    printf("Enter max_performance_time: ");
    scanf("%d", &t2);
    fflush(stdout);

    printf("Enter MAX WAITING TIME: ");
    scanf("%d", &patience_time);

    nump_a = 0, nump_e = 0, nump_ae = 0, nump_s = 0;
    //https://www.man7.org/linux/man-pages/man3/sem_init.3.html
    // debug(num_stage_a);
    // debug(num_stage_e);
    sem_init(&sem_empty_a, 0, num_stage_a);
    sem_init(&sem_empty_e, 0, num_stage_e);
    sem_init(&sem_filled_ae, 0, 0);
    sem_init(&rogue_sem, 0, 0);

    sem_init(&sem_tshirt_givers, 0, (unsigned int)(num_coordinators));
}

int main()
{
    int i;
    srand(time(0));
    take_input();
    // char inp_helper[100];
    tot_num_stages = num_stage_a + num_stage_e;
    for (i = 0; i < tot_num_stages; i++)
    {

        // enum stage_statuses
        // {
        //     Unoccupied,
        //     one_musician,
        //     one_singer,
        //     two_folks
        // };
        // printf("Taking stage stuff\n");
       // debug(i);
        //fflush(stdout);
        st_ptr[i] = (struct stage *)malloc(sizeof(struct stage));

        st_ptr[i]->stage_id = i;
        st_ptr[i]->perf_id1 = -1;
        st_ptr[i]->perf_id2 = -1;
        if (i < num_stage_a)
        {
            st_ptr[i]->type = TYPE_A;
        }
        else
        {
            st_ptr[i]->type = TYPE_E;
        }

        st_ptr[i]->curr_stat = Unoccupied;
        pthread_mutex_init(&(st_ptr[i]->mutex), NULL);
    }

    part2;
    part2;
    debug(tot_num_stages);
    debug(tot_num_performers);
    debug(num_coordinators);
    debug(t1);
    debug(t2);
    debug(patience_time);
    part2;
    part2;

    //return 0;

    printf("Enter details in just one line in format <one-word-name> <instrument> <Arrival time>\n");
    printf("Enter details of performers\n");

    for (i = 0; i < tot_num_performers; i++)
    {
        // printf("Want to take input for i as follows : %d\n",i);
        perf_ptr[i] = (struct performer *)malloc(sizeof(struct performer));

        //  printf("Enter name: ");
        scanf("%s", perf_ptr[i]->name);

        //newline stored, so take again
        scanf("%c", &perf_ptr[i]->instrument_id);
        // printf("Enter instrument code: ");
        scanf("%c", &perf_ptr[i]->instrument_id);

        //  printf("Enter arrival time: ");
        scanf(" %d", &perf_ptr[i]->arrival_time);

        // printf("Name is %s\n", perf_ptr[i]->name);
        // printf("Instrument id is %c\n", perf_ptr[i]->instrument_id);
        // printf("Arrival is %d\n", perf_ptr[i]->arrival_time);
        perf_ptr[i]->stage_allotted = -1;
        perf_ptr[i]->type = get_performer_type(perf_ptr[i]->instrument_id);
        perf_ptr[i]->curr_stat = Unarrived;
        perf_ptr[i]->id = i;

        if (perf_ptr[i]->type == perf_a)
        {
            nump_a++;
        }
        else if (perf_ptr[i]->type == perf_e)
        {
            nump_e++;
        }
        else if (perf_ptr[i]->type == perf_ae)
        {
            nump_ae++;
        }
        else if (perf_ptr[i]->type == perf_s)
        {
            nump_s++;
        }

        //printf("name is %s", perf_ptr[i]->name);

        pthread_mutex_init(&(perf_ptr[i]->mutex), NULL);
        perf_ptr[i]->perf_time = get_random_int2(t1, t2, "Gotham City");
        // dispatch_performers(i);
      //  part;
    }
    printf(BGRN"Inputs taken . Simulation starting\n"ANSI_RESET);
    for (i = 0; i < tot_num_performers; i++)
    {
        dispatch_performers(i);
    }

    ////////////////////////////////////////////////////////////////////////////
    // debug(tot_num_performers);
    for (int i = 0; i < tot_num_performers; i++)
    {
        // printf("Will ask %d to join\n",i);
        int performer_type = perf_ptr[i]->type;
        if (performer_type == perf_a)
        {
            pthread_join(perf_ptr[i]->thread_obj[0], NULL);
        }
        else if (performer_type == perf_e)
        {
            pthread_join(perf_ptr[i]->thread_obj[1], NULL);
        }
        else if (performer_type == perf_ae)
        {
            pthread_join(perf_ptr[i]->thread_obj[0], NULL);
            pthread_join(perf_ptr[i]->thread_obj[1], NULL);
        }
        else if (performer_type == perf_s)
        {
            pthread_join(perf_ptr[i]->thread_obj[0], NULL);
            pthread_join(perf_ptr[i]->thread_obj[1], NULL);
            pthread_join(perf_ptr[i]->thread_obj[2], NULL);
        }
        // printf("Joined till id %d\n",i);
        // part2;
    }
    printf(BBLU "----Finished----\nSimulation Over\n" ANSI_RESET);
    fflush(stdout);
    return 0;
}