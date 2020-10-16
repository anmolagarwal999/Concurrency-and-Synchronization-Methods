#include "master_header.h"

int num_hospitals;
int num_companies;
int num_students;
int latest_arrival_permitted = 10;
int tot_conclusions_left;
int hopeful_students_num;
pthread_mutex_t conclusions_mutex;
pthread_mutex_t hopeful_mutex;

void take_input()
{
    printf(ANSI_RED "This text is RED!" ANSI_RESET "\n");
    printf(ANSI_GREEN "This text is GREEN!" ANSI_RESET "\n");
    printf(ANSI_YELLOW "This text is YELLOW!" ANSI_RESET "\n");
    printf(ANSI_BLUE "This text is BLUE!" ANSI_RESET "\n");
    printf(ANSI_MAGENTA "This text is MAGENTA!" ANSI_RESET "\n");
    printf(ANSI_CYAN "This text is CYAN!" ANSI_RESET "\n");

    printf("Enter number of companies:");
    scanf("%d", &num_companies);
    printf("Enter number of vaccination zones:");
    scanf("%d", &num_hospitals);
    printf("Enter number of students: ");
    scanf("%d", &num_students);
    tot_conclusions_left = num_students;
    hopeful_students_num = 0;
    pthread_mutex_init(&conclusions_mutex, NULL);
    pthread_mutex_init(&hopeful_mutex, NULL);
}

int main()
{
    int i, j, k, t;
    printf("Inside main\n");
    take_input();

    // intitialize companies and take their probalities
    for (i = 0; i < num_companies; i++)
    {
        debug(i);
        comp_ptr[i] = (struct company *)malloc(sizeof(struct company));
        scanf("%Lf", &comp_ptr[i]->prob_of_success);
        comp_ptr[i]->id = i;
        comp_ptr[i]->thr_id = pthread_create(&(comp_ptr[i]->thread_obj), NULL, init_company, (void *)(&(comp_ptr[i]->id)));
    }

    printf("Input with companies taken\n");

    //intialize students
    for (i = 0; i < num_students; i++)
    {
        debug(i);
        fflush(stdout);

        stu_ptr[i] = (struct student *)malloc(sizeof(struct student));
        stu_ptr[i]->id = i;
        stu_ptr[i]->curr_stat = -1; //hasn't yet arrived
        stu_ptr[i]->thr_id = pthread_create(&(stu_ptr[i]->thread_obj), NULL, init_student, (void *)(&(stu_ptr[i]->id)));
    }

    printf("Creating hospital threads\n");
    fflush(stdout);
    //initialize hospitals
    for (i = 0; i < num_hospitals; i++)
    {
        debug(i);
        fflush(stdout);

        hosp_ptr[i] = (struct hospital *)malloc(sizeof(struct hospital));
        hosp_ptr[i]->id = i;
        //continue;
        hosp_ptr[i]->thr_id = pthread_create(&(hosp_ptr[i]->thread_obj), NULL, init_hospitals, (void *)(&(hosp_ptr[i]->id)));
    }

    for (int i = 0; i < num_students; i++)
    {
        pthread_join(stu_ptr[i]->thread_obj, NULL);
    }


    for (int i = 0; i < num_companies; i++)
    {
        pthread_join(comp_ptr[i]->thread_obj, NULL);
    }


    for (int i = 0; i < num_hospitals; i++)
    {
        pthread_join(hosp_ptr[i]->thread_obj, NULL);
    }


    printf(ANSI_MAGENTA"Simulation Over"ANSI_RESET);
    fflush(stdout);
}