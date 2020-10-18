#include "master_header.h"

int tot_num_performers;
int nump_a = 0, nump_e = 0, nump_ae = 0, nump_s = 0;
int t1, t2, patience_time;
int num_stage_a, num_stage_e;
int tot_num_stages;
int num_coordinators;

extern sem_t sem_empty_a, sem_empty_e, sem_filled_ae;
sem_t sem_coordinators, sem_tshirt_givers;

int get_performer_type(char ch)
{
    part;
    printf("char input is %c\n", ch);
    part;
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
    printf(ANSI_RED "This text is RED!" ANSI_RESET "\n");
    printf(ANSI_GREEN "This text is GREEN!" ANSI_RESET "\n");
    printf(ANSI_YELLOW "This text is YELLOW!" ANSI_RESET "\n");
    printf(ANSI_BLUE "This text is BLUE!" ANSI_RESET "\n");
    printf(ANSI_MAGENTA "This text is MAGENTA!" ANSI_RESET "\n");
    printf(ANSI_CYAN "This text is CYAN!" ANSI_RESET "\n");

    printf("Enter number of performers:");
    scanf("%d", &tot_num_performers);
    fflush(stdout);

    printf(ANSI_RED "REACHED HERE" ANSI_RESET);

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
    sem_init(&sem_empty_a, 0, num_stage_a);
    sem_init(&sem_empty_e, 0, num_stage_e);
    sem_init(&sem_filled_ae, 0, 0);

    sem_init(&sem_tshirt_givers, 0, (unsigned int)(num_coordinators));
}

int main()
{
    int i, j;
    //srand(time(0));
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
        printf("Taking stage stuff\n");
        debug(i);
        fflush(stdout);
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

    //return 0;

    printf("Enter details of performers\n");
    for (i = 0; i < tot_num_performers; i++)
    {
        perf_ptr[i] = (struct performer *)malloc(sizeof(struct performer));

        printf("Enter details in just one line in format <one-word-name> <instrument> <Arrival time>\n");

        //  printf("Enter name: ");
        scanf("%s", perf_ptr[i]->name);

        //newline stored, so take again
        scanf("%c", &perf_ptr[i]->instrument_id);
        // printf("Enter instrument code: ");
        scanf("%c", &perf_ptr[i]->instrument_id);

        //  printf("Enter arrival time: ");
        scanf(" %d", &perf_ptr[i]->arrival_time);
        printf("Name is %s\n", perf_ptr[i]->name);
        printf("Instrument id is %c\n", perf_ptr[i]->instrument_id);
        printf("Arrival is %d\n", perf_ptr[i]->arrival_time);
        perf_ptr[i]->stage_allotted = -1;
        perf_ptr[i]->type = get_performer_type(perf_ptr[i]->instrument_id);
        perf_ptr[i]->curr_stat = Waiting;
        perf_ptr[i]->id = i;
        //printf("name is %s", perf_ptr[i]->name);

        pthread_mutex_init(&(perf_ptr[i]->mutex), NULL);
        pthread_cond_init(&(perf_ptr[i]->cv), NULL);
        perf_ptr[i]->perf_time = get_rand_int(t1, t2, "Gotham City");
        perf_ptr[i]->curr_stat = Unarrived;
        dispatch_performer(i);
        part;
    }

    ////////////////////////////////////////////////////////////////////////////

    for (int i = 0; i < tot_num_performers; i++)
    {

        pthread_join(perf_ptr[i]->thread_obj, NULL);
    }
    printf(ANSI_MAGENTA "----Finished----\nSimulation Over\n" ANSI_RESET);
    fflush(stdout);
}