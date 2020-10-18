#include "master_header.h"

int tot_num_performers;
int nump_a = 0, nump_e = 0, nump_ae = 0, nump_s = 0;
int t1, t2, patience_time;
int st_num_a, st_num_e;
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
    scanf("%d", &st_num_a);
    fflush(stdout);

    printf("Enter number of electric stages: ");
    scanf("%d", &st_num_e);
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
    int i, j,cnt_sp,ans,len;
    //srand(time(0));
    take_input();
    char inp_helper[100];

    printf("Enter details of performers\n");
    for (i = 0; i < tot_num_performers; i++)
    {
        perf_ptr[i] = (struct performer *)malloc(sizeof(struct performer));

        printf("Enter details in just one line in format <one-word-name> <instrument> <Arrival time>\n");
        // scanf("%s", inp_helper);
        // len = strlen(inp_helper);
        // printf("inputted string is %s",inp_helper);

        cnt_sp = 0;
        ans = 0;

        // for (j = 0; j < len; j++)
        // {
        //     if (inp_helper[j] == ' ')
        //     {
        //         cnt_sp++;
        //         continue;
        //     }
        //     if (cnt_sp == 0)
        //     {
        //         perf_ptr[i]->name[j] = inp_helper[j];
        //         perf_ptr[i]->name[j + 1] = '\0';
        //     }
        //     else if (cnt_sp == 1)
        //     {
        //         perf_ptr[i]->instrument_id = inp_helper[j];
        //     }
        //     else
        //     {
        //         ans*=10;
        //         ans+=(inp_helper[j]-'0');
        //     }
        // }
        // perf_ptr[i]->arrival_time=ans;
        // debug(ans);


        perf_ptr[i]->stage_allotted = -1;
        perf_ptr[i]->type = get_performer_type(perf_ptr[i]->instrument_id);
        perf_ptr[i]->curr_stat = Unarrived;

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

        perf_ptr[i]->thr_id = pthread_create(&(perf_ptr[i]->thread_obj), NULL, performer_entry, (void *)(&(perf_ptr[i]->id)));
        part2;
    }

    for (int i = 0; i < tot_num_performers; i++)
    {

        pthread_join(perf_ptr[i]->thread_obj, NULL);
    }
    printf(ANSI_MAGENTA "----Finished----\nSimulation Over\n" ANSI_RESET);
    fflush(stdout);
}