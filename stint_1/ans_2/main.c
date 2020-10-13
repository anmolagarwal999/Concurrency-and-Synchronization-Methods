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

    //intialize students
    for (i = 0; i < num_students; i++)
    {
        stu_ptr[i] = (struct student *)malloc(sizeof(struct student));
        stu_ptr[i]->id = i;
        stu_ptr[i]->curr_stat = -1; //hasn't yet arrived
        stu_ptr[i]->thr_id = pthread_create(&(stu_ptr[i]->thread_obj), NULL, init_company, (void *)(&(stu_ptr[i]->id)));
    }

    //initialize hospitals
    for (i = 0; i < num_hospitals; i++)
    {
        hosp_ptr[i] = (struct hospital *)malloc(sizeof(struct hospital));
        hosp_ptr[i]->id = i;
        hosp_ptr[i]->thr_id = pthread_create(&(hosp_ptr[i]->thread_obj), NULL, init_company, (void *)(&(hosp_ptr[i]->id)));
    }
}