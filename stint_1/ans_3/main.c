#include "master_header.h"

int tot_num_performers;
int nump_a = 0, nump_e = 0, nump_ae = 0, nump_s = 0;
int t1, t2, patience_time;
int num_stage_a, num_stage_e;
int tot_num_stages;
int num_coordinators;

sem_t sem_a, sem_e, sem_ae, sem_s;
sem_t sem_a_ae_s, sem_e_ae_s;

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
    sem_init(&sem_a, 0, 0);
    sem_init(&sem_e, 0, 0);
    sem_init(&sem_ae, 0, 0);
    sem_init(&sem_s, 0, 0);
    sem_init(&sem_a_ae_s, 0, 0);
    sem_init(&sem_e_ae_s, 0, 0);
}

int main()
{
    int i, j, cnt_sp, ans, len;
    //srand(time(0));
    take_input();
    // char inp_helper[100];

    printf("Enter details of performers\n");
    for (i = 0; i < tot_num_performers; i++)
    {
        perf_ptr[i] = (struct performer *)malloc(sizeof(struct performer));

        printf("Enter details in just one line in format <one-word-name> <instrument> <Arrival time>\n");

        printf("Enter name: ");
        scanf("%s", perf_ptr[i]->name);

        //newline stored, so take again
        scanf("%c", &perf_ptr[i]->instrument_id);
        printf("Enter instrument code: ");
        scanf("%c", &perf_ptr[i]->instrument_id);

        printf("Enter arrival time: ");
        scanf(" %d", &perf_ptr[i]->arrival_time);

        perf_ptr[i]->stage_allotted = -1;
        perf_ptr[i]->type = get_performer_type(perf_ptr[i]->instrument_id);
        perf_ptr[i]->curr_stat = Unarrived;
        perf_ptr[i]->id = i;
        printf("name is %s", perf_ptr[i]->name);

        pthread_mutex_init(&(perf_ptr[i]->mutex), NULL);
        pthread_cond_init(&(perf_ptr[i]->cv), NULL);

        //setting pointers to categories
        if (perf_ptr[i]->type == perf_a)
        {
            musc_a_ptr[nump_a++] = perf_ptr[i];
        }
        else if (perf_ptr[i]->type == perf_e)
        {
            musc_e_ptr[nump_e++] = perf_ptr[i];
        }
        else if (perf_ptr[i]->type == perf_ae)
        {
            musc_ae_ptr[nump_ae++] = perf_ptr[i];
        }
        else if (perf_ptr[i]->type == perf_s)
        {
            singer_ptr[nump_s++] = perf_ptr[i];
        }

        // perf_ptr[i]->thr_id = pthread_create(&(perf_ptr[i]->thread_obj), NULL, performer_entry, (void *)(&(perf_ptr[i]->id)));
        part2;
    }

    for (i = 0; i < tot_num_performers; i++)
    {

        perf_ptr[i]->thr_id = pthread_create(&(perf_ptr[i]->thread_obj), NULL, stage_entry, (void *)(&(perf_ptr[i]->id)));
        part2;
    }
    ////////////////////////////////////////////////////////////////////////////

    for (i = 0; i < tot_num_stages; i++)
    {

        st_ptr[i]->stage_id = i;
        st_ptr[i]->perf_id1 = -1;
        st_ptr[i]->perf_id2 = -1;
        if (i < num_stage_a)
        {
            st_ptr[i]->type = stage_type_a;
        }
        else
        {
            st_ptr[i]->type = stage_type_e;
        }

        st_ptr[i]->curr_stat=0;
        st_ptr[i]->thr_id = pthread_create(&(st_ptr[i]->thread_obj), NULL, performer_entry, (void *)(&(st_ptr[i]->stage_id)));
    }

    ////////////////////////////////////////////////////////////////////////////

    for (int i = 0; i < tot_num_performers; i++)
    {

        pthread_join(perf_ptr[i]->thread_obj, NULL);
    }
    for (int i = 0; i < tot_num_stages; i++)
    {

        pthread_join(st_ptr[i]->thread_obj, NULL);
    }
    printf(ANSI_MAGENTA "----Finished----\nSimulation Over\n" ANSI_RESET);
    fflush(stdout);
}