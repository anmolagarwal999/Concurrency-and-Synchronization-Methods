#include "master_header.h"

int num_hospitals;
int num_companies;
int num_students;
int latest_arrival_permitted = 15;
int tot_conclusions_left;
int hopeful_students_num;
pthread_mutex_t conclusions_mutex;
pthread_mutex_t hopeful_mutex;

int w_min_prepare_time = 2, w_max_prepare_time = 5;
int r_min_val = 1, r_max_val = 5;
int p_min_val = 10, p_max_val = 20;

void take_input()
{
    printf("Enter number of companies:");
    scanf("%d", &num_companies);
    printf("Enter number of vaccination zones:");
    scanf("%d", &num_hospitals);
    printf("Enter number of students: ");
    scanf("%d", &num_students);
    tot_conclusions_left = num_students;

    //hopeful students initially is zero as no student has yet arrived to be hopeful
    hopeful_students_num = 0;
    pthread_mutex_init(&conclusions_mutex, NULL);
    pthread_mutex_init(&hopeful_mutex, NULL);

    if (num_companies == 0)
    {
        printf(ANSI_RED "No company is present to manufacture vaccine\n" ANSI_RESET);
        printf(BMAG "Simulation Over\n" ANSI_RESET);

        exit(0);
    }

    if (num_hospitals == 0)
    {
        printf(ANSI_RED "No vaccination zone is present to administer vaccine\n" ANSI_RESET);
        printf(BMAG "Simulation Over\n" ANSI_RESET);

        exit(0);
    }

    if (num_students == 0)
    {
        printf(ANSI_RED "No students are coming to college it seems. No need of vaccine camps\n" ANSI_RESET);
        printf(BMAG "Simulation Over\n" ANSI_RESET);

        exit(0);
    }
}

int main()
{
    int i;
    srand(time(0));

    //printf("Inside main\n");
    take_input();

    // intitialize companies and take their probalities
    for (i = 0; i < num_companies; i++)
    {

        comp_ptr[i] = (struct company *)malloc(sizeof(struct company));
        scanf("%Lf", &comp_ptr[i]->prob_of_success);
        comp_ptr[i]->capacity_of_batches = 0;
        comp_ptr[i]->curr_batches_num = 0;
        comp_ptr[i]->left_batches_num = 0;
        pthread_mutex_init(&(comp_ptr[i]->mutex), NULL);
        pthread_cond_init(&(comp_ptr[i]->cv), NULL);

        comp_ptr[i]->id = i;
    }
    for (i = 0; i < num_companies; i++)
    {
        comp_ptr[i]->thr_id = pthread_create(&(comp_ptr[i]->thread_obj), NULL, init_company, (void *)(&(comp_ptr[i]->id)));
    }

    printf("Input with companies taken\n");

    //intialize students
    for (i = 0; i < num_students; i++)
    {

        stu_ptr[i] = (struct student *)malloc(sizeof(struct student));
        stu_ptr[i]->id = i;
        stu_ptr[i]->curr_stat = -1; //hasn't yet arrived
        pthread_mutex_init(&(stu_ptr[i]->mutex), NULL);
        stu_ptr[i]->thr_id = pthread_create(&(stu_ptr[i]->thread_obj), NULL, init_student, (void *)(&(stu_ptr[i]->id)));
    }

    printf("Creating hospital threads\n");
    fflush(stdout);
    //initialize hospitals
    for (i = 0; i < num_hospitals; i++)
    {

        hosp_ptr[i] = (struct hospital *)malloc(sizeof(struct hospital));
        hosp_ptr[i]->id = i;
        //continue;
        hosp_ptr[i]->thr_id = pthread_create(&(hosp_ptr[i]->thread_obj), NULL, init_hospitals, (void *)(&(hosp_ptr[i]->id)));
    }

    for (int i = 0; i < num_students; i++)
    {
        pthread_join(stu_ptr[i]->thread_obj, NULL);
    }

    printf("STUDENTS HAVE JOINED\n");
    fflush(stdout);

    for (int i = 0; i < num_companies; i++)
    {
        printf("Waking comp with id %d\n", i);
        pthread_mutex_lock(&(comp_ptr[i]->mutex));
        pthread_cond_signal(&(comp_ptr[i]->cv));
        pthread_mutex_unlock(&(comp_ptr[i]->mutex));
        pthread_join(comp_ptr[i]->thread_obj, NULL);
        printf("comp with id TERMINATED%d\n", i);
    }

    printf("COMPANIES HAVE JOINED\n");
    fflush(stdout);

    for (int i = 0; i < num_hospitals; i++)
    {

        pthread_join(hosp_ptr[i]->thread_obj, NULL);
    }

    printf("HOSPITALS HAVE JOINED\n");
    fflush(stdout);

    printf(BMAG "Simulation Over\n" ANSI_RESET);
    fflush(stdout);
}